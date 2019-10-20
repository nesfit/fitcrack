<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <v-container class="max500">
    <fc-tile
      title="Mask sets"
      class="ma-2"
    >
      <v-alert
        :value="true"
        type="warning"
        class="mt-0 mb-1"
      >
        Mask files must have a .txt or .hcmask extension.
      </v-alert>
      <v-data-table
        :headers="headers"
        :items="masks.items"
        :loading="loading"
        :footer-props="{itemsPerPageOptions: [10,25,50], itemsPerPageText: 'Masks per page'}"
      >
        <template
          slot="items"
          slot-scope="props"
        >
          <td>
            <router-link :to="{name: 'maskDetail', params: { id: props.item.id}}">
              {{ props.item.name }}
            </router-link>
          </td>
          <td class="text-right">
            {{ $moment(props.item.time ).format('DD.MM.YYYY HH:mm') }}
          </td>
          <td class="text-right">
            <a
              :href="$serverAddr + '/masks/' + props.item.id + '/download'"
              target="_blank"
            >
              <v-btn
                outlined
                fab
                small
                color="primary"
              >
                <v-icon>file_download</v-icon>
              </v-btn>
            </a>
          </td>
          <td class="text-right">
            <v-tooltip top>
              <template v-slot:activator="{ on }">
                <v-btn
                  icon
                  class="mx-0"
                  @click="deleteMask(props.item.id)"
                  v-on="on"
                >
                  <v-icon color="error">
                    delete
                  </v-icon>
                </v-btn>
              </template>
              <span>Delete mask file</span>
            </v-tooltip>
          </td>
        </template>
      </v-data-table>
      <v-divider />
      <file-uploader
        :url="this.$serverAddr + '/masks/add'"
        @uploadComplete="loadMasks"
      />
    </fc-tile>
  </v-container>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  import FileUploader from "@/components/fileUploader/fileUploader";
  export default {
    name: "MasksView",
    components: {
      FileUploader,
      'fc-tile': tile,
    },
    data: function () {
      return {
        loading: false,
        masks: [],
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Added', value: 'time', align: 'right'},
          {text: 'Download', value: 'name', align: 'right'},
          {text: 'Delete', align: 'right'}
        ]
      }
    },
    mounted: function () {
      this.loadMasks()
    },
    methods: {
      loadMasks: function () {
        this.loading = true;
        this.axios.get(this.$serverAddr + '/masks', {}).then((response) => {
          this.masks = response.data;
          this.loading = false
        })
      },
      deleteMask: function (id) {
        this.$root.$confirm('Delete', 'Are you sure?', { color: 'primary' }).then((confirm) => {
          this.loading = true;
          this.axios.delete(this.$serverAddr + '/masks/' + id).then((response) => {
            this.loadMasks()
          })
        })
      }
    }
  }
</script>

<style scoped>
  .noEvent {
    pointer-events: none;
    display: inline-block;
  }

  .dz-message {
    cursor: pointer;
    text-align: center;
  }

  .max500 {
    max-width: 600px;
  }

</style>


<style>
  .selectedDict {
    background: rgba(37, 157, 173, 0.85) !important;
    color: white;
  }

  .selectedDict a {
    color: white;
  }

  .clickable {
    cursor: pointer;
  }
</style>
