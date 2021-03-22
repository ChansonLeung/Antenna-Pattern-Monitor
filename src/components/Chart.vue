<template>
  <div ref="chart" class="fill"></div>
</template>

<script>
const lcjs = require("@arction/lcjs");

export default {
  name: "Chart",
  props:['data_p'],
  created:function(){ 
    
  },
  
  mounted: function () {
      const {
        lightningChart,
        SurfaceSeriesTypes3D,
        LUT,
        ColorRGBA,
        SurfaceMeshSeries3D,
        PalettedFill,
        ColorHSV,
        lut,
        numberOfColumns,
        IndividualPointFill,
        SolidFill,
      } = lcjs;

      const chart3D = lightningChart()
        .Chart3D({
          container: this.$refs.chart,
        })
        .setTitle("Antenna Pattern");

      chart3D.getDefaultAxisX().setInterval(-1, 1).setTitle("X轴");
      chart3D.getDefaultAxisY().setInterval(-1, 1).setTitle("Y轴");
      chart3D.getDefaultAxisZ().setInterval(-1, 1).setTitle("Z轴");

    

      const rows = 178;
      const columns = 88;
      
      const surface = chart3D.addSurfaceSeries({
        type: SurfaceSeriesTypes3D.Mesh,
        rows,
        columns,
        start: { x: 0, z: 0 },
        end: { x: 178, z: 88 },
        pixelate: true,
      });

      // Create color Look-Up-Tables for dynamic colouring.
      const paletteValue = new LUT({
        steps: [
          { value: 0, color: ColorRGBA(255, 255, 255) },
          { value: 10, color: ColorRGBA(255, 0, 0) },
        ],
        interpolate: true,
      });


      var cor_value =[]
      for(var index in this.xx){
          var cor_value_i = []
          for(var indexj in this.xx[index]){
              cor_value_i[indexj] = Math.pow((Math.pow(this.xx[index][indexj],2)+Math.pow(this.yy[index][indexj],2)+Math.pow(this.zz[index][indexj],2)),0.5)
          }
          cor_value[index] = cor_value_i
      }
        surface.invalidateValuesOnly(
         cor_value   
        )

      //上色
      surface.setFillStyle(
        new PalettedFill({ lut: paletteValue, lookUpProperty: "value" })
      );
      

      // Define Mesh geometry as a function of:  (row, column) => { x, y, z }
      const algrithum = require('../algrithum')
      var result = algrithum.Cal_Pattern(0,0,0)
      var pitch = 0
      setInterval( ()=>{
        var result = algrithum.Cal_Pattern(this.data_p.yaw,this.data_p.pitch,0)
        //画图
        surface
          .invalidateGeometryOnly((row, column, prev) => {
            return {
              x: result.xx.valueOf()[column][row],
              y: result.yy.valueOf()[column][row],
              z: result.zz.valueOf()[column][row],
            };
          })
          .setWireframeStyle(new SolidFill({ color: ColorRGBA(0, 0, 0, 80) }));

      } ,2000)
  },
};
</script>
<style scoped>
.fill {
  height: 100%;
}
</style>