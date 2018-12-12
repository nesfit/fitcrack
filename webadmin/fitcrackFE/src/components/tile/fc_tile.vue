<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-layout row wrap class=" mx-auto elevation-2 white">
    <v-toolbar color="primary" dark card class="max64">
      <v-toolbar-title>{{title}}</v-toolbar-title>
    </v-toolbar>
    <div class="contentFcTile">
      <v-progress-circular
        size="50"
        :width="3"
        class="progress"
        indeterminate
        color="primary"
        v-if="loading"
      ></v-progress-circular>
      <slot v-else></slot>
    </div>
  </v-layout>
</template>

<script>
  export default {
    props: ['title', 'loading'],
    name: "fc_tile"

  }
</script>

<style scoped>

  .white {
    background: white;
    min-width: 300px;
    position: relative;
  }

  .contentFcTile {
    position: relative;
    min-height: 100px;
    width: 100%;
  }

  .progress {
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    margin: auto;
    position: absolute;
  }

  .max64 {
    max-height: 64px;
  }

</style>
