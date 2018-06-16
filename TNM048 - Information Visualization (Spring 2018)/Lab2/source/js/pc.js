/*
  Created: Jan 25 2018
  Authors: Kahin Akram Hassan, Prithiviraj Muthumanickam
*/
function pc(data1,data2,data3){

  //Chage data set
  var data = data3;

  var div = '#pc-chart';


  var parentWidth = $(div).parent().width();
  var margin = {top: 40, right: 10, bottom: 10, left: 40},
      width = parentWidth - margin.left - margin.right,
      height = 600 - margin.top - margin.bottom;

  var x = d3.scaleBand().range([0, width]),
      y = {};

  var colors = colorbrewer.Set2[6];
  var scale = d3.scaleLinear().range([height, 0]);

  var line = d3.line(),
      axis = d3.axisLeft(),
      background,
      foreground;

  var svg = d3.select(div).append("svg")
         .attr("width", width + margin.right + margin.left)
         .attr("height", height + margin.top + margin.bottom)
         .append("svg:g")
         .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

   // Extract the list of dimensions and create a scale for each.
   x.domain(dimensions = d3.keys(data[0]).filter(function(d) {
       return (y[d] = d3.scaleLinear()
           .domain(d3.extent(data, function(p) { return +p[d]; }))
           .range([height, 0])
           );
   }));

   var k = 7;
   var kmeansRes = kmeans(data,k);
   draw(kmeansRes);

   function draw(kmeansRes){

     // Add grey background lines for context.
     background = svg.append("g")
         .attr("class", "background")
         .selectAll("path")
         .data(data)
         .enter().append("path")
         .attr("d", path);

     // Add blue foreground lines for focus.
     foreground = svg.append("g")
         .attr("class", "foreground")
         .selectAll("path")
         .data(data)
         .enter().append("path")
         .attr("d", path)
         .style("stroke", function(d) { return "hsl(" + Math.random() * 360 + ",100%,50%)"; })
         .style("stroke", function(d, i) { return colors[kmeansRes[i].assignments]; });


     // Add a group element for each dimension.
     var g = svg.selectAll(".dimension")
          .data(dimensions)
          .enter().append("g")
          .attr("class", "dimension")
          .attr("transform", function(d) { return "translate(" + x(d) + ")"; });



      // Add an axis and title.
      g.append("g")
          .attr("class", "axis")
         .each(function(d) { d3.select(this).call(d3.axisLeft(y[d])); })
          .append("text")
          .attr("text-anchor", "middle")
          .attr("y", -9)
          .style('fill','black')
          .text(String);

      // Add and store a brush for each axis.
      g.append("g")
          .attr("class", "brush")
          .each(function(d) {
            d3.select(this).call(d.brush = d3.brushY()
            .extent([[-10,0], [10,height]])
            .on("start", brushstart)
            .on("brush", brush)
            .on("end", brush)
          );
          })
          .selectAll("rect")
          .attr("x", -8)
          .attr("width", 10);


      function brushstart() {
        d3.event.sourceEvent.stopPropagation();
      }

      // Handles a brush event, toggling the display of foreground lines.
      function brush() {

        var actives = [];
        svg.selectAll(".dimension .brush")
        .filter(function(d) {
          return d3.brushSelection(this);
        })
        .each(function(d) {
          actives.push({
            dim: d,
            extent: d3.brushSelection(this)
          });
        });
        var a = [];
        foreground.style("display", function (d) {
            return actives.every(function (active) {
               var dim = active.dim;
               var ext = active.extent;
               return  within(d, ext, dim,a);
            }) ? null : "none";
        });

        function within(d, extent, dim, a) {
          return scale(d[dim]) > extent[0]  && scale(d[dim]) < extent[1];
        };

      }//end of brush

      // Returns the path for a given data point.
      function path(d) {
          return line(dimensions.map(function(p) { return [x(p), y[p](d[p])]; }));
      }//end of path

   } // end of draw
} // end of pc
