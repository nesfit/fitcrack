<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
    <v-container>
      <v-layout row wrap>
        <v-flex xs12>
          <test-element>

          </test-element>
        </v-flex>
      </v-layout>
    </v-container>
</template>

<script>
  import element from '@/components/logs/logsView'
  export default {
    name: "test",
    components: {
      'test-element': element
    },
    data() {
      return {
      }
    },
  }
</script>

<style scoped>

</style>
