module.exports = {
  transpileDependencies: [
    'vuetify'
  ],
  configureWebpack: config => {
    
    config.devtool = 'source-map'
  }
}
