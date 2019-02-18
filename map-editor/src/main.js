import Vue from 'vue'
import './plugins/vuetify'
import App from './App.vue'
import splitPane from 'vue-splitpane'
import * as THREE from 'three'


Vue.config.productionTip = false

const MODES =  [ "Map editor", "Block Editor" ];

export const three = {
  scenes: {
    [MODES[0]]: { 
      scene: new THREE.Scene(),
      map: {
        width: 10,
        height: 10
      },
    },
    [MODES[1]]: { 
      scene: new THREE.Scene(),
    },
  },
  current: () => three.scenes[three.mode],
  scene: (mode) => three.scenes[three.modes[mode]],
  modes: MODES,
  mode: MODES[0],
};

Vue.component('split-pane', splitPane);

new Vue({
  data: function() {
    return {
    };
  },
  render: h => h(App)
}).$mount('#app')
