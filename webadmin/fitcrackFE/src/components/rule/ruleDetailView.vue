<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs
      v-if="data != null"
      divider="/"
      class="pb-0"
    >
      <v-breadcrumbs-item>
        <router-link :to="{name: 'rules'}">
          Rules
        </router-link>
      </v-breadcrumbs-item>
      <v-breadcrumbs-item>
        {{ data.name }}
      </v-breadcrumbs-item>
    </v-breadcrumbs>

    <v-container>
      <v-row justify="center">
        <fc-tile
          title="Rule file"
          :loading="data==null"
          class="mx-2 dictContentContainer mb-4"
        >
          <v-list
            v-if="data != null"
            single-line
            class="width100"
          >
            <v-list-item class="px-2 py-1">
              <v-list-item-action class="pr-3 key">
                Name:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-right">
                  {{ data.name }}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
            <v-divider />
            <v-list-item class="px-2 py-1">
              <v-list-item-action class="pr-3 key">
                Added:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-right">
                  {{ $moment(data.time).format('DD.MM.YYYY HH:mm') }}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
            <v-divider />
            <v-list-item class="px-2 py-1">
              <v-list-item-content>
                <v-row class="width100 margintop5">
                  <v-spacer />
                  <a
                    :href="$serverAddr + '/rule/' + data.id + '/download'"
                    target="_blank"
                  >
                    <v-btn
                      class="ma-0"
                      outlined
                      color="primary"
                    >
                      Download <v-icon class="ml-2">file_download</v-icon>
                    </v-btn>
                  </a>
                </v-row>
              </v-list-item-content>
            </v-list-item>
          </v-list>
          <v-divider />
          <div
            v-if="data != null"
            class="dictContent"
          >
            <fc-textarea
              :readonly="true"
              :search-enable="true"
              :url="this.$serverAddr + '/rule/' + this.$route.params.id + '/data'"
              max-height="500"
            />
          </div>
        </fc-tile>
      </v-row>
    </v-container>
  </div>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  import fcTextarea from '@/components/textarea/fc_textarea'
  export default {
    name: "RuleDetailView",
    components: {
      'fc-tile': tile,
      'fc-textarea': fcTextarea
    },
    data: function () {
      return {
        data: null
      }
    },
    mounted: function () {
      this.loadData()
    },
    methods: {
      loadData: function ($state) {
        this.axios.get(this.$serverAddr + '/rule/' + this.$route.params.id).then((response) => {
          this.data = response.data
        });
      }
    }
  }
</script>

<style scoped>
  .dictContentContainer {
    max-width: 800px;
  }
  .dictContent {
    overflow: auto;
  }

  .width100 {
    width: 100%;
  }

  .margintop5 {
    margin-top: 10px;
  }
</style>
