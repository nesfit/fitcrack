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
      <v-icon left>note_add</v-icon>Make template
    </v-btn>

    <v-card>
      <v-card-title
        class="headline grey lighten-2"
        primary-title
      >
        Job template
      </v-card-title>

      <v-card-text>
        <!-- <json-viewer :value="data" ></json-viewer> -->
        <v-text-field
          name="templateName"
          label="Template name"
          id="templateName"
          v-model="data.name"
        ></v-text-field>

        <div class="flex">
          <v-icon left>info</v-icon>
          <div>
            The choices you made in the job configuration will be saved as a template.<br>
            You can pre-fill any new job configuration with templates you saved.
          </div>
        </div>
      </v-card-text>

      <v-divider></v-divider>

      <v-card-actions>
        <v-spacer></v-spacer>
        <v-btn
          flat
          @click="dialog = !dialog"
        >Back to editing</v-btn>
        <v-btn
          color="primary"
          flat
          @click="submit"
        >
          Save template
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
.flex {
  display: flex;
}
</style>
