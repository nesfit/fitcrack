<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs divider="/" class="pb-0"  v-if="data != null">
      <v-breadcrumbs-item>
        <router-link :to="{name: 'rules'}">Rules</router-link>
      </v-breadcrumbs-item>
      <v-breadcrumbs-item>
        {{data.name}}
      </v-breadcrumbs-item>
    </v-breadcrumbs>

    <v-container>
      <v-layout row wrap justify-center>
        <fc-tile title="Rule file" :loading="data==null" class="mx-2 dictContentContainer mb-4">
          <v-list single-line class="width100" v-if="data != null">
            <v-list-item class="px-2 py-1">
              <v-list-item-action class="pr-3 key">
                Name:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-xs-right">
                  {{data.name}}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
            <v-divider></v-divider>
            <v-list-item class="px-2 py-1">
              <v-list-item-action class="pr-3 key">
                Added:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-xs-right">
                  {{ $moment(data.time).format('DD.MM.YYYY HH:mm') }}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
            <v-divider></v-divider>
            <v-list-item class="px-2 py-1">
              <v-list-item-content>
                <v-layout row class="width100 margintop5">
                  <v-spacer></v-spacer>
                  <a :href="$serverAddr + '/rule/' + data.id + '/download'" target="_blank">
                    <v-btn class="ma-0" outlined color="primary">
                      Download <v-icon class="ml-2">file_download</v-icon>
                    </v-btn>
                  </a>
                </v-layout>
              </v-list-item-content>
            </v-list-item>
          </v-list>
          <v-divider></v-divider>
          <div class="dictContent" v-if="data != null">
            <fc-textarea :readonly="true" :searchEnable="true" :url="this.$serverAddr + '/rule/' +  this.$route.params.id + '/data'" maxHeight="500"></fc-textarea>
          </div>

        </fc-tile>
      </v-layout>
    </v-container>
  </div>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  import fcTextarea from '@/components/textarea/fc_textarea'
  export default {
    name: "ruleDetailView",
    components: {
      'fc-tile': tile,
      'fc-textarea': fcTextarea
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
    },
    data: function () {
      return {
        data: null
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
