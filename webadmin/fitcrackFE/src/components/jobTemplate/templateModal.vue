<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-dialog
    v-model="dialog"
    width="800"
  >
    <v-btn
      slot="activator"
      large
      color="primary"
    >
      Make template
    </v-btn>

    <v-card>
      <v-card-title
        class="headline grey lighten-2"
        primary-title
      >
        Job template
      </v-card-title>

      <v-card-text>
        <json-viewer :value="data" ></json-viewer>
      </v-card-text>

      <v-divider></v-divider>

      <v-card-actions>
        <v-spacer></v-spacer>
        <v-btn
          color="primary"
          flat
          @click="submit"
        >
          Submit template
        </v-btn>
      </v-card-actions>
    </v-card>
  </v-dialog>
</template>

<script>
  import jsonViewer from '@/components/json/json-viewer'
  export default {
    name: "templateModal",
    components: {
      'json-viewer': jsonViewer
    },
    props: {
      data: {
        type: Object,
        default: null
      },
    },
    methods: {
      submit() {
        console.log(this.data)
        this.loading = true
        this.axios.post(this.$serverAddr + '/template', this.data).then((response) => {
          console.log(response.data)
        }).catch((error) => {
          this.loading = false
        })
        this.dialog = false
      }
    },
    data() {
      return {
        dialog: false
      }
    },
  }
</script>

<style scoped>

</style>
