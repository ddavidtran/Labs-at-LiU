#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>
#include <mpi.h>

#include "definitions.h"
#include "physics.h"

float rand1(){
	return (float)( rand()/(float) RAND_MAX );
}


int main(int argc, char** argv){
	unsigned time_max;
    unsigned send_buf_size, recv_buf_size;
    int rank,world_size;
    MPI_Status mpi_status;
	float pressure = 0;
    float global_pressure = 0;
    double s_time, e_time;
    std::vector<Particle> particles;
    std::vector<POD_Particle> send_up_buf;
    std::vector<POD_Particle> send_down_buf;
    std::vector<POD_Particle> recv_buf;
    Wall wall;

	srand( time(NULL) + 1234 );

	// parse arguments
	if(argc != 2) {
		std::cerr << "Usage: "<< argv[0] << " simulation_time\n";
		exit(1);
	}
	time_max = atoi(argv[1]);

    // Init MPI
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Saves the rank in rank
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); // Saves the number of processes in world_size

    MPI_Datatype MPI_Particle;

    MPI_Type_contiguous(4, MPI_FLOAT, &MPI_Particle);
    MPI_Type_commit(&MPI_Particle);


    // Init wall
    wall = Wall(0,BOX_HORIZ_SIZE,0,BOX_VERT_SIZE/world_size,rank == 0,rank == world_size -1);
    // Init particles
	for(int i = 0; i < INIT_NO_PARTICLES/world_size; i++){
		float r = rand1()*MAX_INITIAL_VELOCITY;
		float a = rand1()*2*PI;
        particles.emplace_back(wall.x0 + rand1()*(wall.x1-wall.x0),
                               wall.y0 + rand1()*(wall.y1-wall.y0),
                               r*std::cos(a),r*std::sin(a));
	}

    s_time = MPI::Wtime();

	/* Main loop */
    int ccw = 0;
	for (unsigned time_stamp = 0; time_stamp < time_max; time_stamp++) { // for each time stamp
        // Reset collision for all particles
        for (auto& p : particles) {
            p.collided = false;
        }

        // Reset buffers
        send_up_buf.clear();
        send_down_buf.clear();
        recv_buf.clear();

        // check for collisions with other particles
		for (unsigned i=0; i<particles.size(); ++i) { // for all particles
			if (particles[i].collided) continue;
            // Check every particle after current particle i in vector
			for (unsigned j=i+1; j<particles.size(); ++j) {
				if (particles[j].collided) continue;
				float t = collide(particles[i], particles[j]);
				if (t!=-1) { // collision
                    particles[i].collided = true;
                    particles[j].collided = true;
					interact(particles[i], particles[j], t);
					break; // only check collision of two particles
				}
			}
		}
		// move particles that has not collided with another
        // add momentum to pressure if collision with wall
        POD_Particle tmp;
		for (auto p_it = particles.begin(); p_it != particles.end();) {
			if (!p_it->collided) {
				feuler(*p_it, 1);
                float dp = wall_collide(*p_it, wall);
                pressure += dp;
                if (dp != 0.0) ccw++;
                // If particle above this area send to neighbour above
                if (p_it->y < wall.y0 && !wall.upsolid) {
                    tmp.x = p_it->x;
                    tmp.y = p_it->y;
                    tmp.vx = p_it->vx;
                    tmp.vy = p_it->vy;
                    send_up_buf.push_back(tmp);
                    p_it = particles.erase(p_it);
                } else if (p_it->y > wall.y1 && !wall.downsolid) {
                    tmp.x = p_it->x;
                    tmp.y = p_it->y;
                    tmp.vx = p_it->vx;
                    tmp.vy = p_it->vy;
                    send_down_buf.push_back(tmp);
                    p_it = particles.erase(p_it);
                } else {
                    ++p_it;
                }
			} else {
                ++p_it;
            }
        }


        // Send particles to process rank-1
        send_buf_size = send_up_buf.size();
        if (rank != 0) {
            MPI_Send(&send_buf_size,1,MPI_UNSIGNED,rank-1,1000, MPI_COMM_WORLD);
            if (!send_up_buf.empty()) {
                MPI_Send(&send_up_buf[0], send_up_buf.size(), MPI_Particle, rank-1, 1001, MPI_COMM_WORLD);
            }
        }

        // Receive particles from rank+1
        if (rank != world_size-1) {
            MPI_Recv(&recv_buf_size,1,MPI_UNSIGNED,rank+1,1000, MPI_COMM_WORLD, &mpi_status);
            if (recv_buf_size != 0) {
                recv_buf.resize(recv_buf_size);
                MPI_Recv(&recv_buf[0], recv_buf_size, MPI_Particle, rank+1, 1001, MPI_COMM_WORLD, &mpi_status);
                for (unsigned i = 0; i < recv_buf_size; ++i)
                    particles.push_back(Particle(recv_buf[i]));
            }
        }


        // Send particles to process rank+1
        send_buf_size = send_down_buf.size();
        if (rank != world_size-1) {
            MPI_Send(&send_buf_size,1,MPI_UNSIGNED,rank+1,2000, MPI_COMM_WORLD);
            if (!send_down_buf.empty()) {
                MPI_Send(&send_down_buf[0], send_down_buf.size(), MPI_Particle, rank+1, 2001, MPI_COMM_WORLD);
            }
        }

        // Receive particles from rank-1
        if (rank != 0) {
            MPI_Recv(&recv_buf_size,1,MPI_UNSIGNED,rank-1,2000, MPI_COMM_WORLD, &mpi_status);
            if (recv_buf_size != 0) {
                recv_buf.resize(recv_buf_size);
                MPI_Recv(&recv_buf[0], recv_buf_size, MPI_Particle, rank-1, 2001, MPI_COMM_WORLD, &mpi_status);
                for (unsigned i = 0; i < recv_buf_size; ++i)
                    particles.push_back(Particle(recv_buf[i]));
            }
        }
	}

    MPI_Reduce(&pressure, &global_pressure, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    e_time = MPI::Wtime();

    if (rank == 0) {
        std::cout << "Average pressure = " << global_pressure / (WALL_LENGTH*time_max) << '\n';
        std::cout << "Elapsed time = " << e_time -s_time << '\n';
    }

    MPI_Finalize();

	return 0;

}
