<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-row class=" mx-auto elevation-2">
    <v-toolbar
      color="primary"
      dark
      flat
      class="max64"
    >
      <v-toolbar-title>{{ title }}</v-toolbar-title>
    </v-toolbar>
    <div class="contentFcTile">
      <v-progress-circular
        v-if="loading"
        size="50"
        :width="3"
        class="progress"
        indeterminate
        color="primary"
      />
      <slot v-else />
    </div>
  </v-row>
</template>

<script>
  export default {
    name: "FcTile",
    props: ['title', 'loading']

  }
</script>

<style scoped>

  .white {

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
