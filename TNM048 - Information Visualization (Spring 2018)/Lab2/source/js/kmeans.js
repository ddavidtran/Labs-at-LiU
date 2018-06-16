/**
* k means algorithm
* @param data
* @param k
* @return {Object}
*/
/************************
 * Function Definitions *
 * **********************
 * function transformData: Convert string numbers to doubles and append extra column to original data to be able to associate every point with closest cluster.
 * function genCentroid: Generate random K points into space as initial cluster.
 * function assignCluster: Assign every data point to the closest centroid.
 * function calculateAverage: Calculate average of data points with same centroid associated.
 * function computeEuclideanDist: Calculate euclidean distance between x and y.
 * function checkClusterQuality: Check quality of cluster by using sum of the squared distance in each cluster.
 */

function transformData(data){
  var dim = Object.keys(data[0])
  data.forEach(function(d){
    d.assignments = 0;
    for(var i = 0; i < dim.length; i++){
      d[dim[i]] = Number(d[dim[i]]);
    }
  })
  return data;
}


function generateCentroid(data, k){ //Returns newly generated centroid based on K points of original data, randomly..
    var centroid = [];
    for(var i = 0; i < k; i++){
      var randIndex = Math.floor(Math.random()*(data.length - 1) + 1);
      centroid[i] = data[randIndex];
    }
    return centroid;
}


//Function to assign data point to closest centroid.
function assignCluster(data, centroid){
    var dim = Object.keys(centroid[0]);

    data.forEach(function(d){
      var minDistance = Infinity;
      var currClosestCentroid = 0; 
      centroid.forEach(function(c, index){
        var eDistance = 0;
        eDistance = computeEuclideanDist(d,c,dim);
        if(eDistance < minDistance){
          minDistance = eDistance;
          currClosestCentroid = index;
        }
     })
     d.assignments = currClosestCentroid; //Found closest centroid for given data point. Associate the data it with the centroid.
    })
}


function calcAverage(data, centroid){
    var dim = Object.keys(centroid[0]);
    centroid.forEach(function(c,index){
      var counter = 0;
      var arr = new Array(dim.length-1).fill(0);
      data.forEach(function(d){
        if(d.assignments == index){
          counter++;
          for(var i = 0; i < dim.length-1; i++){
            arr[i] += d[dim[i]];
          }
        }
      })
      for(var i = 0; i < dim.length-1; i++){
        c[dim[i]] = arr[i]/counter;
      }
    })
}


function computeEuclideanDist(x,y,dim){
  var dist = 0;
  for(var i = 0; i < dim.length; i++){
    dist += Math.sqrt(Math.pow(x[dim[i]]-y[dim[i]],2)); //Euclidean distance
  }
  return dist;
}


function checkClusterQuality(data, centroid){
    var nrCluster = centroid.length; //K-cluster.
    var dim = Object.keys(centroid[0]);
    var qualityArray = [];
    var totQuality = 0;
    for(var i = 0; i < nrCluster; i++){
      var minError = 0;
      data.forEach(function(d){
        if(d.assignments == i){
          minError += computeEuclideanDist(d, centroid[i], dim);
        }
      })
      qualityArray[i] = minError;
    }
    qualityArray.forEach(function (e){ //Total sum of quality.
      totQuality += e;
    })
    return totQuality;
}

function kmeans(data, k) {

    //First transform data and generate centroid with random K-points.
    var data = transformData(data);
    var centroid = generateCentroid(data,k);
    var totalQuality = 0;
    var tempQuality = 0;
    var diffQuality = 0;
    var nrIteration = 0;
    var threshold = 0.01;

    // Task 4.
    do{
      assignCluster(data,centroid); //Assign cluster.
      calcAverage(data,centroid); //Calculate average of data points with same centroid associated.
      totalQuality = checkClusterQuality(data,centroid); //Function returns totalQuality of sum of squared distances for every cluster.
      diffQuality = Math.abs(tempQuality - totalQuality); //Difference between temporary quality and total quality to determine if the quality is improving.
      tempQuality = totalQuality;
      nrIteration++;

    } while(diffQuality > threshold); //If diffQuality is not larger than threshold, then it is good enough.
    console.log("Number of iterations: " + nrIteration);
    return data;
  };  



