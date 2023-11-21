<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs
      v-if="info != null"
      :items="
        [
          { text: 'Hashlists', to: { name: 'hashlists' }, exact: true },
          { text: info.name }
        ]"
      divider="/"
    />

    <v-container>
      <v-card flat class="mb-4">
        <v-card-title>
          {{ info.name }}
        </v-card-title>
        <v-card-text>
          Hash type: <strong>{{ info.hash_type_name }}</strong>
        </v-card-text>
      </v-card>
      <v-card v-if="info.jobs.length > 0" flat class="mb-4">
        <v-card-title>Jobs</v-card-title>
        <v-card-text class="d-flex">
          <miniJob
            v-for="job in info.jobs"
            :key="job.id"
            :data="job"
          >
          </miniJob>
        </v-card-text>
      </v-card>
      <v-data-table
        ref="table"
        :headers="headers"
        :items="info.hashes"
        :search="search"
        :options.sync="pagination"
        :server-items-length="totalItems"
        :loading="loading"
        :footer-props="{itemsPerPageOptions: [25,50,100], itemsPerPageText: 'Hashes per page'}"
      >
        <template v-slot:item.password="{ item }">
          <span v-if="item.password">{{ item.password }}</span>
          <span v-else class="red--text">–</span>
        </template>
        <template v-slot:item.hashText="{ item }">
          <div class="d-flex align-center mw-50">
            <v-dialog max-width="600">
              <template v-slot:activator="{ on }">
                <span 
                  class="text-truncate d-inline-block cursor-pointer"
                  v-on="on"
                >
                  {{ item.hashText }}
                </span>
              </template>
              <v-card>
                <v-card-title>
                  Full hash
                </v-card-title>
                <v-card-text>
                  {{ item.hashText }}
                </v-card-text>
              </v-card>
            </v-dialog>
            <!-- <svg 
              xmlns="http://www.w3.org/2000/svg"
              height="18"
              width="100%"
            >
              <rect
                v-for="(char, i) in item.hashText"
                :key="i"
                :x="i"
                :y="18 + 5 - char.charCodeAt(0) / 5"
                width="1"
                :height="char.charCodeAt(0) / 5 - 5"
                :fill="'hsla(' + (char.charCodeAt(0) * 5 - 100) + ',100%,50%,50%)'"
              />
            </svg> -->
          </div>
        </template>
      </v-data-table>
    </v-container>
  </div>
</template>

<script>
  import fmt from '@/assets/scripts/numberFormat'
  import miniJob from '@/components/dashboard/miniJob.vue'
  export default {
    name: "HashlistDetail",
    components: {
      miniJob
    },
    data: function () {
      return {
        info: {
          hashes: []
        },
        interval: null,
        status: 'active',
        search: '',
        totalItems: 0,
        pagination: {},
        loading: true,
        headers: [
          {text: 'Hash', value: 'hashText', align: 'start', sortable: true},
          {text: 'Password', value: 'password', align: 'end', sortable: true},
        ]
      }
    },
    mounted: function () {
      this.loadData()
    },
    methods: {
      fmt,
      loadData: function ($state) {
        this.axios.get(this.$serverAddr + '/hashlist/' + this.$route.params.id + '/details').then((response) => {
            this.info = response.data
            this.loading = false
            // this needs to be replaced by proper pagination
            this.totalItems = this.info.hashes.length
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
    max-height: 600px;
  }
  .cursor-pointer {
    cursor: pointer
  }

</style>
