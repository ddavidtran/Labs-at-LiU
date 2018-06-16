/*
  Created: Jan 25 2018
  Authors: Kahin Akram Hassan, Prithiviraj Muthumanickam
*/
queue()
  .defer(d3.csv,'source/data/testData1_400x3_2-clusters.csv')
  .defer(d3.csv,'source/data/testData2_400x3_2-clusters.csv')
  .defer(d3.csv,'source/data/testData3_5600x5_x-clusters.csv')
  .await(draw);

var pc;

function draw(error, data1, data2, data3){
  if (error) throw error;

    pc = new pc(data1,data2,data3);

}
