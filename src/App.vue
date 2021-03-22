<template>
  <v-app>
    <v-navigation-drawer app permanent>
    </v-navigation-drawer>
    <v-main>
      <div style="height:100vh">
        <Chart :data_p = "result"></Chart>
      </div>
    </v-main>
  </v-app>
</template>

<script>
import Chart from './components/Chart'

const net = window.require('net')
const buffer = require('buffer')

export default {
  name: 'App',

  components: {
    Chart
  },
  created:function(){
    this.board.createServer()
    this.dymola.connect()
  },
  data: function(){
    return {
    result:{
      yaw:0,
      pitch:0
    },
    board:{
      connected: "未连接",
      socket:null,
      server:null,
      
      send:function(message){
        this.board.socket.write(message)
      }.bind(this),
      createServer:function(){
         //board
        this.board.server = net.createServer();
        this.board.server.listen('10002', "192.168.120.231")
        this.board.server.on('connection', (socket)=>{
          this.board.socket = socket
          console.log('board connect success!')
          this.board.connected = "已连接"

          socket.on('data',(data)=>{
            // directly relay to dymola
            console.log("from board:", data)
            var msg = new Float64Array(data.buffer)

            try {
              this.dymola.send(data)
            } catch (error) {
              console.log(error)
            }
          })
          socket.on('close', ()=>{
            this.board.socket = null 
            console.log('board socket close')
          this.board.connected = "已断开"
          })
        })
      }.bind(this)
    },
    dymola:{
      connected:"未连接",
      socket:null,
      client:null,

      address_config:{
        port:10000,
        ip_address:"192.168.120.231"
      },
      send:function(message){
          this.dymola.socket.write(message)
      }.bind(this),
      connect:function(){
        this.dymola.client = net.Socket()
        this.dymola.socket = this.dymola.client
        
        this.dymola.client.connect(this.dymola.address_config.port, this.dymola.address_config.ip_address, ()=>{
          console.log('dymola connect success')
        })

        //relay to dymola directly
        this.dymola.client.on('data', data=>{
          var msg = new Float64Array(data.buffer)
          this.result.yaw = msg[0]
          this.result.pitch = msg[1]
          console.log("from dymola:",msg)   
          try {
            
            this.board.send(data)
          } catch (error) {
            console.log(error)
          }   
        })

        this.dymola.client.on('close', ()=>{
          console.log('dymola disconected')
        })
      }.bind(this),
      reconnect_Timer:null
    }
  }},

  methods:{
    
  }
};
</script>

<style scoped>
 ::-webkit-scrollbar {
  display: none;
}
</style>
