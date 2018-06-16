import os
import torch
import torch.nn as nn
import torch.nn.functional as F

class LeNet(nn.Module):
    def __init__(self,activation, pooling, pretrained=False):
        super(LeNet, self).__init__()
        self.activation = activation
        self.pooling = pooling 
        
        self.conv1 = nn.Conv2d(3, 6, 5)
        self.conv2 = nn.Conv2d(6, 16, 5)
        self.fc1   = nn.Linear(16*5*5, 120)
        self.fc2   = nn.Linear(120, 84)
        self.fc3   = nn.Linear(84, 10)
        
        if pretrained:
            if os.path.exists('./models/lenet_trained_cifar10_final'):
                pretrained_model = torch.load('./models/lenet_trained_cifar10_final')
                self.load_state_dict(pretrained_model)
                print('Pretrained model was loaded.')
            else:
                print('Pretrained model was not found!')
                    


    def forward(self, x):
        out = self.activation(self.conv1(x))
        out = self.pooling(out, 2)
        out = self.activation(self.conv2(out))
        out = self.pooling(out, 2)
        out = out.view(out.size(0), -1)
        out = self.activation(self.fc1(out))
        out = self.activation(self.fc2(out))
        out = self.fc3(out)
        return out
