<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-dialog
    v-model="dialog"
    width="800"
  >
    <template #activator="{ on }">
      <v-btn
        large
        color="primary"
        v-on="on"
      >
        <v-icon left>
          mdi-file-plus
        </v-icon>Make template
      </v-btn>
    </template>

    <v-card>
      <v-card-title
        primary-title
      >
        Job template
      </v-card-title>

      <v-card-text>
        <!-- <json-viewer :value="data" ></json-viewer> -->
        <div class="flex mb-3">
          <v-icon left>
            info
          </v-icon>
          <div>
            The choices you made in the job configuration will be saved as a template.<br>
            You can pre-fill any new job configuration with templates you saved.
          </div>
        </div>
        <v-text-field
          id="templateName"
          v-model="data.name"
          name="templateName"
          outlined
          dense
          hide-details
          label="Template name"
        />
      </v-card-text>

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
