package tddc17;


import aima.core.environment.liuvacuum.*;
import aima.core.agent.Action;
import aima.core.agent.AgentProgram;
import aima.core.agent.Percept;
import aima.core.agent.impl.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.Random;

class MyAgentState
{
	public int[][] world = new int[30][30];
	public int initialized = 0;
	final int UNKNOWN 	= 0;
	final int WALL 		= 1;
	final int CLEAR 	= 2;
	final int DIRT		= 3;
	final int HOME		= 4;
	final int ACTION_NONE 			= 0;
	final int ACTION_MOVE_FORWARD 	= 1;
	final int ACTION_TURN_RIGHT 	= 2;
	final int ACTION_TURN_LEFT 		= 3;
	final int ACTION_SUCK	 		= 4;
	
	public int agent_x_position = 1;
	public int agent_y_position = 1;
	public int agent_last_action = ACTION_NONE;
	
	public static final int NORTH = 0;
	public static final int EAST = 1;
	public static final int SOUTH = 2;
	public static final int WEST = 3;
	public int agent_direction = EAST;
	
	MyAgentState()
	{
		for (int i=0; i < world.length; i++)
			for (int j=0; j < world[i].length ; j++)
				world[i][j] = UNKNOWN;
		world[1][1] = HOME;
		agent_last_action = ACTION_NONE;
	}
	// Based on the last action and the received percept updates the x & y agent position
	public void updatePosition(DynamicPercept p)
	{
		Boolean bump = (Boolean)p.getAttribute("bump");

		if (agent_last_action==ACTION_MOVE_FORWARD && !bump)
	    {
			switch (agent_direction) {
			case MyAgentState.NORTH:
				agent_y_position--;
				break;
			case MyAgentState.EAST:
				agent_x_position++;
				break;
			case MyAgentState.SOUTH:
				agent_y_position++;
				break;
			case MyAgentState.WEST:
				agent_x_position--;
				break;
			}
	    }
		
	}
	
	public void updateWorld(int x_position, int y_position, int info)
	{
		world[x_position][y_position] = info;
	}
	
	public void printWorldDebug()
	{
		for (int i=0; i < world.length; i++)
		{
			for (int j=0; j < world[i].length ; j++)
			{
				if (world[j][i]==UNKNOWN)
					System.out.print(" ? ");
				if (world[j][i]==WALL)
					System.out.print(" # ");
				if (world[j][i]==CLEAR)
					System.out.print(" . ");
				if (world[j][i]==DIRT)
					System.out.print(" D ");
				if (world[j][i]==HOME)
					System.out.print(" H ");
			}
			System.out.println("");
		}
	}
}

class MyAgentProgram implements AgentProgram {
	
	//OUR STUFF
	
	LinkedList<Action> actionQueue = new LinkedList<Action>();
	LinkedList<int[]> path = new LinkedList<int[]>();
	LinkedList<int[]> uStack = new LinkedList<int[]>();
	LinkedList<int[]> visited = new LinkedList<int[]>();
	int[] goal = {};
	int[] start = {};

	//____________________________
	
	
	private int initnialRandomActions = 10;
	private Random random_generator = new Random();
	
	// Here you can define your variables!
	static int TURNS = 20*20*2;
	public int iterationCounter = TURNS;
	public MyAgentState state = new MyAgentState();
	
	// moves the Agent to a random start position
	// uses percepts to update the Agent position - only the position, other percepts are ignored
	// returns a random action
	private Action moveToRandomStartPosition(DynamicPercept percept) {
		int action = random_generator.nextInt(6);
		initnialRandomActions--;
		state.updatePosition(percept);
		if(action==0) {
		    state.agent_direction = ((state.agent_direction-1) % 4);
		    if (state.agent_direction<0) 
		    	state.agent_direction +=4;
		    state.agent_last_action = state.ACTION_TURN_LEFT;
			return LIUVacuumEnvironment.ACTION_TURN_LEFT;
		} else if (action==1) {
			state.agent_direction = ((state.agent_direction+1) % 4);
		    state.agent_last_action = state.ACTION_TURN_RIGHT;
		    return LIUVacuumEnvironment.ACTION_TURN_RIGHT;
		} 
		state.agent_last_action=state.ACTION_MOVE_FORWARD;
		return LIUVacuumEnvironment.ACTION_MOVE_FORWARD;
	}
	
	
	@Override
	public Action execute(Percept percept) {
		
		// DO NOT REMOVE this if condition!!!
    	if (initnialRandomActions>0) {
    		return moveToRandomStartPosition((DynamicPercept) percept);
    	} else if (initnialRandomActions==0) {
    		// process percept for the last step of the initial random actions
    		initnialRandomActions--;
    		state.updatePosition((DynamicPercept) percept);
			//System.out.println("Processing percepts after the last execution of moveToRandomStartPosition()");
			state.agent_last_action=state.ACTION_SUCK;
			System.out.println("DONE WITH RANDOM");

			//uStack = checkNearby(new int[] {state.agent_x_position, state.agent_y_position}, visited, uStack);
	    	return LIUVacuumEnvironment.ACTION_SUCK;
    	}
		
    	// This example agent program will update the internal agent state while only moving forward.
    	// START HERE - code below should be modified!
    	 
    	/*
    	System.out.println("x=" + state.agent_x_position);
    	System.out.println("y=" + state.agent_y_position);
    	System.out.println("dir=" + state.agent_direction);
    	*/
		
	    iterationCounter--;
	    
	    if (iterationCounter==0)
	    	return NoOpAction.NO_OP;

	    DynamicPercept p = (DynamicPercept) percept;
	    Boolean bump = (Boolean)p.getAttribute("bump");
	    Boolean dirt = (Boolean)p.getAttribute("dirt");
	    Boolean home = (Boolean)p.getAttribute("home");
	    //System.out.println("percept: " + p);
	    
	    // State update based on the percept value and the last action
	    state.updatePosition((DynamicPercept)percept);
	    if (bump) {
			switch (state.agent_direction) {
			case MyAgentState.NORTH:
				state.updateWorld(state.agent_x_position,state.agent_y_position-1,state.WALL);
				break;
			case MyAgentState.EAST:
				state.updateWorld(state.agent_x_position+1,state.agent_y_position,state.WALL);
				break;
			case MyAgentState.SOUTH:
				state.updateWorld(state.agent_x_position,state.agent_y_position+1,state.WALL);
				break;
			case MyAgentState.WEST:
				state.updateWorld(state.agent_x_position-1,state.agent_y_position,state.WALL);
				break;
			}
	    }
	    if (dirt)
	    	state.updateWorld(state.agent_x_position,state.agent_y_position,state.DIRT);
	    else
	    	state.updateWorld(state.agent_x_position,state.agent_y_position,state.CLEAR);
	    
	    //state.printWorldDebug();
	    
	    
	    // Next action selection based on the percept value
	    if (dirt)
	    {
	    	//System.out.println("DIRT -> choosing SUCK action!");
	    	state.agent_last_action=state.ACTION_SUCK;
	    	return LIUVacuumEnvironment.ACTION_SUCK;
	    } 
	    else
	    {
	    	if (bump)
	    	{
	    		state.agent_last_action=state.ACTION_NONE;
		    //	return NoOpAction.NO_OP;
	    	}
	    	//else
	    	//{	
	    		if(iterationCounter == TURNS-1) {
    				uStack.addAll(checkNearby(new int[] {state.agent_x_position, state.agent_y_position}, visited, uStack));
	    		}
	    		if(path.isEmpty() && actionQueue.isEmpty()) {
	    			if(uStack.isEmpty()) {
		    			System.out.println("GO HOME!");
		    			goal = new int[] {1,1};
		    			start = new int[] {state.agent_x_position, state.agent_y_position};
	    				state.printWorldDebug();  
	    				path = AStar(start, goal);
    				}
	    			else {
	    				uStack.addAll(checkNearby(new int[] {state.agent_x_position, state.agent_y_position}, visited, uStack));
	    				for(int[] i : uStack) {
		    				//System.out.println("STACK:" + i[0] + ":" + i[1]);
		    			}	
		    			start = new int[] {state.agent_x_position, state.agent_y_position};
		    			if(!compareTo(visited,start))
		    				visited.add(start);
		    			goal = uStack.pollLast();

	    				path = AStar(start, goal);
	    				}
    			}
	    		if(actionQueue.isEmpty()) {
    				System.out.println("CURRENT POSITION: " + state.agent_x_position + ":" + state.agent_y_position+"\n");
    				System.out.println("GOAL NODE: " + goal[0] + ":" + goal[1] + "\n");
    				for(int[] i : path) {
	    				System.out.println("PATHS:" + i[0] + ":" + i[1]);
	    			}
	    			int[] lookVector = directionToVector(state.agent_direction);
	    			int[] next = path.poll();
	    			if(!compareTo(visited,next))
	    				visited.add(next);
	    				
	    			//System.out.println("POLLED PATH: " + next[0] + ":" + next[1]);
	    			int[] walkVector = {next[0] - state.agent_x_position, next[1] - state.agent_y_position};

	    			actionQueue = generateActionQueue(lookVector, walkVector);
	    		}
	    		
    			Action actionToDo = actionQueue.poll();
    			int actionInt = actionToInt(actionToDo);
    			updateDirection(actionToDo);
    			state.agent_last_action = actionInt;
    			
    			return actionToDo;
	    	}
	    }
	//}
	
	
	/*if(!uStack.isEmpty())
	if(!uStack.isEmpty() && actionQueue.isEmpty()) {
		for(int[] i : uStack) {
			System.out.println("STACK:" + i[0] + ":" + i[1]);
		}
		start = new int[] {state.agent_x_position, state.agent_y_position};
		goal = uStack.pollLast();
		System.out.print("NEW GOAL NODE: " + goal[0] + " : " + goal[1]);
	}
	
	else if(uStack.isEmpty() && actionQueue.isEmpty()){
		System.out.println("WOW");
		goal = new int[] {1,1};
		start = new int[] {state.agent_x_position, state.agent_y_position};
	}
	
	if(path.isEmpty())
		path = AStar(start, goal);
		for(int[] i : path) {
			//System.out.println("PATHS:" + i[0] + ":" + i[1]);
		}
	
	if(actionQueue.isEmpty()) {
		//TEMPORARY, pervent Crash (nullPointerException)
		if(path.isEmpty())
			return NoOpAction.NO_OP;
		System.out.println("CURRENT POSITION: " + state.agent_x_position + ":" + state.agent_y_position);

		visited.add(new int[] {state.agent_x_position, state.agent_y_position});
		//System.out.println("USTACK SIZE BEFORE: " + uStack.size());
		uStack.addAll(checkNearby(new int[] {state.agent_x_position, state.agent_y_position}, visited, uStack));

		//System.out.println("USTACK SIZE AFTER: " + uStack.size());
		int[] lookVector = directionToVector(state.agent_direction);
		int[] next = path.poll();
		int[] walkVector = {next[0] - state.agent_x_position, next[1] - state.agent_y_position};

		actionQueue = generateActionQueue(lookVector, walkVector);
	}*/
	
	/*
	fn = gn + hn
	gn = cost(start, nextNode)
	hn = manhattan(nextNode, goal)
	 */
	
	private LinkedList<int[]> AStar(int[] start, int[] goal) {
		
		LinkedList<int[]> path = new LinkedList<int[]>();
		int pathCost = 0;
		LinkedList<int[]> visited = new LinkedList<int[]>();
		
		path.add(start);
		visited.add(start);
		
		
		while(!path.isEmpty() && !Arrays.equals(path.peekLast(), goal)) {
			int[] bestNextStep = {};
			int lowestCost = (int)Double.POSITIVE_INFINITY;
			int[] current = path.peekLast();			
			ArrayList<int[]> directions = new ArrayList<int[]>();
				directions.add(new int[] {0, -1});
				directions.add(new int[] {1, 0});
				directions.add(new int[] {0, 1});
				directions.add(new int[] {-1, 0});
				
			for(int[] nextDirection : directions) {
				int fn, gn, hn = 0;
				int[] next = {current[0] + nextDirection[0], current[1] + nextDirection[1]};	
				if(!compareTo(visited,next) && state.world[next[0]][next[1]] != state.WALL) {
					gn = pathCost + 1;
					hn = calcManhattan(next, goal);
					fn = gn + hn;
					if(fn < lowestCost) {
						lowestCost = fn;
						bestNextStep = next;
                        
                    }
				}
			}
			
			if(bestNextStep.length == 0)
				path.pop();
			else {
				path.add(bestNextStep);
				pathCost++;
				visited.add(bestNextStep);
			}
		}		

		path.removeFirst();
		return path;
	}
	
	private LinkedList<Action> generateActionQueue(int[] lookVector, int[] walkVector){
		LinkedList<Action> actionQueue = new LinkedList<Action>();
		int[] from = lookVector;
		int[] to = walkVector;
		
		int angle = getAngle(from, to);
		
		int v21 = from[0] * to[1];
		int v12 = from[1] * to[0];
		
		if(v21 < 0)
			angle*=-1;
		if(v12 > 0)
			angle*=-1;
		
		if(angle == 0) {
			actionQueue.add(LIUVacuumEnvironment.ACTION_MOVE_FORWARD);
		}
		else if(angle == 180) {
			actionQueue.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			actionQueue.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			actionQueue.add(LIUVacuumEnvironment.ACTION_MOVE_FORWARD);
		}
		else if(angle > 0) {
			actionQueue.add(LIUVacuumEnvironment.ACTION_TURN_RIGHT);
			actionQueue.add(LIUVacuumEnvironment.ACTION_MOVE_FORWARD);
		}
		else {
			actionQueue.add(LIUVacuumEnvironment.ACTION_TURN_LEFT);
			actionQueue.add(LIUVacuumEnvironment.ACTION_MOVE_FORWARD);
		}
		
		
		return actionQueue;
	}
	
	
	private int getAngle(int[] v1, int[] v2) {	
		v2[0] = (int) (v2[0] / Math.sqrt(Math.pow(v2[0], 2) + Math.pow(v2[1], 2)));
		v2[1] = (int) (v2[1] / Math.sqrt(Math.pow(v2[0], 2) + Math.pow(v2[1], 2)));
  		double taljare = ((v1[0] * v2[0]) + (v1[1] * v2[1]));
		double namnare = ((Math.sqrt(Math.pow(v1[0], 2) + Math.pow(v1[1], 2))) * (Math.sqrt(Math.pow(v2[0], 2)) + Math.pow(v2[1], 2)));
		int angle = (int) Math.toDegrees(Math.acos(taljare/namnare));
		return angle;
	}
	
	
	
	private int calcManhattan(int[] current, int[] goal) {
		return Math.abs(current[0] - goal[0]) + Math.abs(current[1] - goal[1]);		
	}
	
	
	
	private void updateDirection(Action lastAction) {
		int lastActionInt = actionToInt(lastAction);
			
		if(lastActionInt == state.ACTION_TURN_RIGHT)
		{
			state.agent_direction += 1;
			if(state.agent_direction > 3)
				state.agent_direction = 0;
		}
		else if(lastActionInt == state.ACTION_TURN_LEFT) {
			state.agent_direction -= 1;
			if(state.agent_direction < 0)
				state.agent_direction = 3;
		}
	}
	
	
	private int actionToInt(Action a) {
		if(a.equals(LIUVacuumEnvironment.ACTION_TURN_RIGHT))
			return state.ACTION_TURN_RIGHT;
		else if(a.equals(LIUVacuumEnvironment.ACTION_TURN_LEFT))
			return state.ACTION_TURN_LEFT;
		else if(a.equals(LIUVacuumEnvironment.ACTION_MOVE_FORWARD))
			return state.ACTION_MOVE_FORWARD;
		else if(a.equals(LIUVacuumEnvironment.ACTION_SUCK))
			return state.ACTION_SUCK;
		else
			return state.ACTION_NONE;
	}
	
	
	private int[] directionToVector(int dir) {
		if		(dir == state.NORTH) { return new int[] {0, -1}; }
		else if	(dir == state.SOUTH) { return new int[] {0, 1};  }
		else if	(dir == state.WEST)	 { return new int[] {-1, 0}; }
		else						 { return new int[]  {1, 0};  }
	}
	
	
	private LinkedList<int[]> checkNearby(int[] pos, LinkedList<int[]> visited, LinkedList<int[]> uStack){
		LinkedList<int[]> storeUnknowns = new LinkedList<int[]>();
		int x = pos[0];
		int y = pos[1];
		ArrayList<int[]> directions = new ArrayList<int[]>();
		directions.add(new int[] {x,y-1});
		directions.add(new int[] {x,y+1});
		directions.add(new int[] {x-1, y});
		directions.add(new int[] {x+1, y});
		
		for(int[] p: directions) {
			if(state.world[p[0]][p[1]] == state.UNKNOWN && !compareTo(visited,p) && !compareTo(uStack, p)) {
				storeUnknowns.add(p);
			}
		}
		return storeUnknowns;
	}
	
	private boolean compareTo(List<int[]> list, int[] item) {
		for(int[] i : list) {
		if(Arrays.equals(i,item)) {
				return true;
			}
		}
		return false;
	}
	
}

public class MyVacuumAgent extends AbstractAgent {
    public MyVacuumAgent() {
    	super(new MyAgentProgram());
	}
}
