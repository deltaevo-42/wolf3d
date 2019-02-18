<template>
  <v-app>
    <v-content>
      <v-layout style="height: 60vh">
        <v-flex xs12>
          <v-select solo :items="three.modes" v-model="three.mode" dark class="absolute-selector"></v-select>
          <r-canvas></r-canvas>
        </v-flex>
      </v-layout>
      <controls></controls>
    </v-content>
  </v-app>
</template>

<script>
import Canvas from '@/components/Canvas.vue'
import controls from '@/components/controls.vue'
import {three} from '@/main'

export default {
  name: 'App',
  components: {
    'r-canvas': Canvas,
    controls
  },
  data () {
    return {
      three,
    }
  },
  methods: {

  }
}
</script>


<style>
.absolute-selector {
  position: absolute;
  right: 5px;
  top: 5px;
  max-width: 200px;
}
</style>
