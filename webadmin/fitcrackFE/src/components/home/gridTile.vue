<template>
  <div class="d-flex flex-column ma-4 grid-tile">
    <v-icon 
      large
      :color="color"
      class="mb-2"
    >
      {{ icon }}
    </v-icon>
    <slot />
  </div>
</template>

<script>
export default {
  props: {
    icon: {
      type: String,
      default: 'mdi-checkbox-blank-circle-outline'
    },
    color: {
      type: String,
      default: ''
    }
  }
}
</script>

<style scoped>
.grid-tile {
  width: 12ch;
}
</style>