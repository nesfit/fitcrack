<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-row class=" mx-auto elevation-2">
    <v-toolbar
      flat
      class="max64"
    >
      <v-toolbar-title class="d-flex align-center">
        <v-icon 
          v-if="icon"
          left
        >
          {{ icon }}
        </v-icon>
        {{ title }}
      </v-toolbar-title>
    </v-toolbar>
    <div class="contentFcTile">
      <v-skeleton-loader
        v-if="loading"
        class="mx-auto"
        type="article"
      />
      <slot v-else />
    </div>
  </v-row>
</template>

<script>
  export default {
    name: "FcTile",
    props: {
      title: String,
      loading: Boolean,
      icon: String
    }
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
