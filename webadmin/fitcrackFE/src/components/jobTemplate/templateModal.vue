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
      <v-icon left>
        note_add
      </v-icon>Make template
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
          id="templateName"
          v-model="data.name"
          name="templateName"
          label="Template name"
        />

        <div class="flex">
          <v-icon left>
            info
          </v-icon>
          <div>
            The choices you made in the job configuration will be saved as a template.<br>
            You can pre-fill any new job configuration with templates you saved.
          </div>
        </div>
      </v-card-text>

      <v-divider />

      <v-card-actions>
        <v-spacer />
        <v-btn
          text
          @click="dialog = !dialog"
        >
          Back to editing
        </v-btn>
        <v-btn
          color="primary"
          text
          @click="submit"
        >
          Save template
        </v-btn>
      </v-card-actions>
    </v-card>
  </v-dialog>
</template>

<script>
  export default {
    name: "TemplateModal",
    props: {
      data: {
        type: Object,
        default: null
      },
    },
    data() {
      return {
        dialog: false
      }
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
  }
</script>

<style scoped>
.flex {
  display: flex;
}
</style>
