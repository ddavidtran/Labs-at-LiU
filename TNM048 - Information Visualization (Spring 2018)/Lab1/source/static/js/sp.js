/*
  Created: Jan 14 2018
  Author: Kahin Akram Hassan
*/

function sp(data){

    this.data = data;
    var div = '#scatter-plot';
    var hashmap = d3.map();
    var flag = 1;
    var height = 500;
    var parentWidth = $(div).parent().width();
    var margin = {top: 20, right: 20, bottom: 60, left: 40},
        width = parentWidth - margin.right - margin.left,
        height = height - margin.top - margin.bottom;

    var color = d3.scaleOrdinal(d3.schemeCategory20);

    var tooltip = d3.select(div).append("div")
        .attr("class", "tooltip")
        .style("opacity", 0);

    var x = d3.scaleLinear().range([0, width]);
    var y = d3.scaleLinear().range([height, 0]);
    console.log(x);
    /* Task 2
      Initialize 4 (x,y,country,circle-size)
      variables and assign different data attributes from the data filter
      Then use domain() and extent to scale the axes x and y domain code here*/

    var country = d3.scaleOrdinal(); //Scale ordinal since the data attributes are not numerical.
    var circle_size = "Student_skills";
    var xValue = "Life_satisfaction";
    var yValue = "Household_income";


    x.domain(d3.extent(this.data, function(d) { return d[xValue]; })).nice();
    y.domain([0,d3.max(data, function(d){ return d[yValue]})]).nice();
    country.domain(d3.extent(this.data, function(d) {return d["Country"]; }));

    var svg = d3.select(div).append("svg")
                          .attr("width", width + margin.left + margin.right)
                          .attr("height", height + margin.top + margin.bottom)
                          .append("g")
                          .attr("transform","translate(" + margin.left + "," + margin.top + ")");

    /* ~~ Task 3 Add the x and y Axis and title  ~~ */
    var xAxis = d3.axisBottom(x);
    var yAxis = d3.axisLeft(y);

    //Add the x-axis
    svg.append("g")
     .attr("transform", "translate(0," + height + ")") //Align axis at the bottom of svg.
     .call(xAxis);

    //Text label for x-axis
    svg.append("text")             
      .attr("transform","translate(" + (width/2) + " ," + (height + margin.top + 20) + ")")
      .style("text-anchor", "middle")
      .text("Employment_rate");

    //Add the y-axis
    svg.append("g")
       .call(yAxis);

    //Text label for y-axis
    svg.append("text")
      .attr("transform", "rotate(-90)") //y-axis text has to be rotated.
      .attr("y", 0 - margin.left) //Align text before the axis.
      .attr("x",0 - (height / 2)) //Align text in the middle.
      .attr("dy", "1em")
      .style("text-anchor", "middle")
      .text("Student_skills"); 

    /* ~~ Task 4 Add the scatter dots. ~~ */
    //Attr cx, cy and r. Use svg.selectAll
    var circles = svg.selectAll(".circle")
                    .data(this.data)
                    .enter().append("circle")
                    .attr("class", "dot")
                    .attr("class", "non_brushed") //Task 5.
                    .attr("r", function(d){ return d[circle_size]/10;}) //Radius of the dot.
                    .attr("cx", function(d) { return x(d[xValue]); }) // x-axis coordinate of the center of the element.
                    .attr("cy", function (d) { return y(d[yValue]); }) // y-axis coordinate of the center of the element.
                    .style("fill", function(d) { return color(d["Country"]); });

    /* ~~ Task 5 create the brush variable and call highlightBrushedCircles() ~~ */
    var brush = d3.brush().on("brush", highlightBrushedCircles);
    svg.append("g")
      .call(brush);

         //highlightBrushedCircles function
         function highlightBrushedCircles() {
             if (d3.event.selection != null) {
                 // revert circles to initial style
                 circles.attr("class", "non_brushed");
                 var brush_coords = d3.brushSelection(this);
                 // style brushed circles
                   circles.filter(function (){
                            var cx = d3.select(this).attr("cx");
                            var cy = d3.select(this).attr("cy");
                            return isBrushed(brush_coords, cx, cy);
                  })
                  .attr("class", "brushed");
                   var d_brushed =  d3.selectAll(".brushed").data();
                   /* ~~~ Call pc or/and map function to filter ~~~ */
                  pc.selectLine(d_brushed);
                  map.selectCountry(d_brushed);
                   
             }
         }//highlightBrushedCircles
         function isBrushed(brush_coords, cx, cy) {
              var x0 = brush_coords[0][0],
                  x1 = brush_coords[1][0],
                  y0 = brush_coords[0][1],
                  y1 = brush_coords[1][1];
             return x0 <= cx && cx <= x1 && y0 <= cy && cy <= y1;
         }//isBrushed

         //Select all the dots filtered
         this.selectDots = function(value){
            //For map.js
            if(typeof value == 'string'){
              svg.selectAll('.non_brushed').each(function(d){
                if(d.Country == value){
                  d3.select(this).filter(function(d){
                    return d.Country == value;         
                  })
                    .style("stroke-width", 4)
                    .style("stroke", "black");
                }
                else
                  d3.select(this).style("stroke", "none");
                }
              )
            }

            //For pc.js
            else{
              svg.selectAll('.non_brushed').each(function(d){
                //console.log(map.size())
                if(d.Country == value.Country && !hashmap.has(value.Country)){
                    hashmap.set(value.Country, value.Country)
                    d3.select(this).filter(function(d){
                    return d.Country == value.Country;         
                  })
                    .style("stroke-width", 4)
                    .style("stroke", "black");
                }
                else if(hashmap.has(value.Country) && d.Country != value.Country){
                    hashmap.remove(value.Country);
                    d3.select(this).style("stroke", "none");
                  }
                })
              }
          };


}//End
