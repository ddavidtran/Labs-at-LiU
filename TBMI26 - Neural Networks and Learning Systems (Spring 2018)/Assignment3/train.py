import torch
import torch.nn as nn
from torch.autograd import Variable
import os 


### Train the network 
#Start training the network for a number of epochs (here we set it to 50). In each epoch: 
#- We loop over all images in the training set
#- Feed them forward through the network
#- Calculate the loss after the last layer.
#- Backpropagate the loss until the first layer.
#- Update the weights for different convolution layers based on the gradients.

def train(model, trainloader, optimizer, objective, use_cuda, start_epoch, num_epochs):
    loss_log = []
    acc_log = [] 

    for epoch in range(start_epoch, num_epochs+1):

        print('\nEpoch: %d' % epoch)

        model.train()

        # Variables for training status
        train_loss = 0
        correct = 0
        total = 0

        # Loop on all images in the dataset
        for batch_idx, (inputs, targets) in enumerate(trainloader):

            # Move data to GPU if CUDA is available
            if use_cuda: 
                inputs, targets = inputs.cuda(), targets.cuda()

            # Convert input Tensor datatype to Variable which is trainable and has a gradient
            inputs, targets = Variable(inputs), Variable(targets)

            # Clear the gradients of all variable 
            optimizer.zero_grad()

            # Feed-forward the network
            outputs = model(inputs)

            # Calculate the loss
            loss = objective(outputs, targets)

            # Feed Backward
            loss.backward()
            # Update the weights
            optimizer.step()

            # Update training status
            train_loss += loss.data[0]

            # Find the class with the highest output
            _, predicted = torch.max(outputs.data, 1)

            # Number of total images trained so far and the correctly classified ones
            total += targets.size(0)
            correct += predicted.eq(targets.data).cpu().sum()

            print('Loss: {:.8f} | Acc: {:.2f}% ({}/{})'.format((train_loss/(batch_idx+1)), 100.*correct/total, correct, total))

        loss_log.append((train_loss/(batch_idx+1)))
        acc_log.append(100.*correct/total)

        # Save a checkpoint when the epoch finishes
        state = {
            'epoch': epoch,
            'net': model.state_dict(),
            'optimizer' : optimizer.state_dict(),
        }
        if not os.path.isdir('./checkpoints'):
            os.mkdir('./checkpoints')
        file_path = './checkpoints/checkpoint_{}.ckpt'.format(epoch)
        torch.save(state, file_path)

    # Save the final model    
    torch.save(model.state_dict(), './models/lenet_trained_cifar10_final')
    print('Training Finished!')
    return model, loss_log, acc_log