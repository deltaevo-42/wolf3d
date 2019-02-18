<template>
    <div ref="canvas" style="height: 100%"></div>
</template>

<script>
import * as THREE from 'three'

import {three} from '@/main'

const OrbitControls = require('three-orbit-controls')(THREE)

export default {
    name: 'Canvas',
    data() {
        return {
            three,
            needLoad: true
        };
    },
    mounted() {
        console.log("called", this.three.scene(0));
        this.three.camera = new THREE.PerspectiveCamera(75, this.$refs.canvas.offsetWidth /  this.$refs.canvas.offsetHeight, 0.1, 1000);
        this.three.renderer = new THREE.WebGLRenderer();
        this.three.renderer.setSize(this.$refs.canvas.offsetWidth, this.$refs.canvas.offsetHeight);
        this.three.controls = new OrbitControls(this.three.camera, this.three.renderer.domElement);
        this.$refs.canvas.append(this.three.renderer.domElement);

        this.three.scene(1).cube = {};
        this.three.scene(1).cube.geometry = new THREE.BoxGeometry( 1, 1, 1 );
        this.three.scene(1).cube.material = new THREE.MeshBasicMaterial( { color: 0x00ff00 } );
        this.three.scene(1).cube.cube = new THREE.Mesh(  this.three.scene(1).cube.geometry,  this.three.scene(1).cube.material );
        this.three.scene(1).scene.add( this.three.scene(1).cube.cube );

        this.three.scene(0).plane = {};
        this.three.scene(0).plane.geometry = new THREE.PlaneBufferGeometry( this.three.scene(0).map.width, this.three.scene(0).map.height, this.three.scene(0).map.width, this.three.scene(0).map.height );
        this.three.scene(0).plane.material = new THREE.MeshBasicMaterial( {color: 0x252525, side: THREE.DoubleSide} );
        this.three.scene(0).plane.plane = new THREE.Mesh( this.three.scene(0).plane.geometry,  this.three.scene(0).plane.material );
        this.three.scene(0).scene.add( this.three.scene(0).plane.plane );

        this.three.camera.position.z = 5;

        window.addEventListener('resize', this.resize)
        this.three.controls.update();
        this.render();
    }, 
    beforeDestroy: function () {
        window.removeEventListener('resize', this.resize);
        this.needLoad = false;
    },
    methods: {
        resize() {
            console.log("Called")
            this.three.camera.aspect = this.$refs.canvas.offsetWidth / this.$refs.canvas.offsetHeight;
            this.three.camera.updateProjectionMatrix();
            this.three.renderer.setSize(this.$refs.canvas.offsetWidth, this.$refs.canvas.offsetHeight);
        },
        render() {
            this.three.renderer.render(this.three.current().scene, this.three.camera);
            this.three.controls.update();
            if (this.needLoad)
                requestAnimationFrame(this.render)
        }
    },
}
</script>
