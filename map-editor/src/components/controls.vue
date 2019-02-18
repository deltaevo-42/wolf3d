<template>
    <div class="controls">
        <template v-if="three.mode == three.modes[0]">
            <!-- <Split style="height: 40vh" :gutterSize="5">
                <SplitArea :size="70">
                    <v-layout>
                        <v-flex xs6>
                            <v-text-field label="width" v-model="three.scene(0).map.width" @change="updateSize"></v-text-field>
                        </v-flex>
                        <v-flex xs6>
                            <v-text-field label="height" v-model="three.scene(0).map.height" @change="updateSize"></v-text-field>
                        </v-flex>
                    </v-layout>
                </SplitArea>
                <SplitArea :size="30">
                    <blocks-list/>
                </SplitArea>
            </Split> -->
        </template>
        <template v-if="three.mode == three.modes[1]">
               <split-pane  style="height: 40vh"  :default-percent='50' split="vertical">
                    <template slot="paneL">
                        A
                    </template>
                    <template slot="paneR">
                        <split-pane :default-percent='50' split="vertical">
                            <template slot="paneL" >
                                <textures-list style="overflow-y: scroll; height: 40vh"/>
                            </template>
                            <template slot="paneR">
                                <blocks-list style="overflow-y: scroll; height: 40vh"/>
                            </template>
                        </split-pane>
                    </template>
                </split-pane>
            <!-- <Split style="height: 40vh">
                <SplitArea :size="33">
                
                </SplitArea>
                <SplitArea :size="33">
                    <textures-list/>
                </SplitArea>
                <SplitArea :size="33">
                    <blocks-list/>
                </SplitArea>
            </Split> -->
        </template>
    </div>
</template>

<script>
import {three} from '@/main'
import BlocksList from '@/components/BlocksList'
import TexturesList from '@/components/TexturesList'

import * as THREE from 'three'

export default {
    name: 'controls',
    components: {
        BlocksList,
        TexturesList
    },
    data() {
        return {
            three
        }
    },
    methods: {
        updateSize() {
           console.log(this.three.scene(0))
           this.three.scene(0).plane.geometry = new THREE.PlaneBufferGeometry(three.scene(0).map.width, three.scene(0).map.height, three.scene(0).map.width, three.scene(0).map.height);
           this.three.scene(0).plane.plane.geometry = this.three.scene(0).plane.geometry;
        }
    }
}
</script>
