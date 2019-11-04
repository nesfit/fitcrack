<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-card class="mx-auto">
    <v-card-title>
      <v-icon 
        v-if="icon"
        left
      >
        {{ icon }}
      </v-icon>
      {{ title }}
    </v-card-title>
    <v-card-text class="contentFcTile">
      <v-skeleton-loader
        v-if="loading"
        class="mx-auto"
        type="article"
      />
      <slot v-else />
    </v-card-text>
  </v-card>
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
    padding: 0;
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
