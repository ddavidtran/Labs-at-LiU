%% Main
clc
clear

world = 4; % Explore which world 1-4.
l_rate = 0.1; % Learning rate.
d_factor = 0.9; % Discount factor.
eps = 0.9 % Exploration factor.
actionVec = [1 2 3 4]; % Action vector.
probAction = [1 1 1 1]; % Probability distribution for each action.
nrEpisodes = 10000; % Number of episodes.

gwinit(world)
s = gwstate;
Qtable = zeros(s.xsize, s.ysize, length(actionVec)); % Initialize Qtable.

gwdraw;

for x = 1:nrEpisodes
    tic
    if mod(x,nrEpisodes/10) == 0
        (x/nrEpisodes) * 100
    end
    
    %Generate new random position and state.
    gwinit(world);
    s = gwstate();
    
    %While we have not found the goal state.
    while(s.isterminal == 0)
        [action, ~] = chooseaction(Qtable, s.pos(1), s.pos(2), actionVec, probAction, eps);
        pos_state = gwaction(action);
        xPos = pos_state.pos(1);
        yPos = pos_state.pos(2);
        
        if pos_state.isvalid == 0
            r = -0.05;
            Qtable(s.pos(1),s.pos(2), action) = (1-l_rate)*Qtable(s.pos(1),s.pos(2), action) + ...
                                                 l_rate*(r + ...
                                                 d_factor*max(Qtable(s.pos(1), s.pos(2), :)));
        else
            r = pos_state.feedback;
            Qtable(s.pos(1),s.pos(2),action) = (1-l_rate)*Qtable(s.pos(1),s.pos(2), action) + ...
                                                 l_rate*(pos_state.feedback + ...
                                                 d_factor*max(Qtable(pos_state.pos(1), pos_state.pos(2), :)));
            s = pos_state;
        end
    end
    
    Qtable(s.pos(1),s.pos(2),:) = 0; %Goal        
    eps = 1 - exp((x/nrEpisodes) - 1);
    
end
time = toc
s = gwstate;
figure(1)
gwdraw();
for x = 1:s.xsize
    for y = 1:s.ysize
        [~,optA] = max(Qtable(x, y, actionVec));
        gwplotarrow([x,y], optA);
    end
end


