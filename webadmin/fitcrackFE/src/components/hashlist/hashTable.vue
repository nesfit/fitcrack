<template>
  <v-container>
    <v-toolbar flat v-if="!noToolbar">
      <v-toolbar-title class="mr-4">Hashes</v-toolbar-title>
      <v-spacer></v-spacer>
      <v-btn-toggle v-model="crackStatus" dense @change="loadHashes">
        <v-btn value="All" text>All</v-btn>
        <v-btn value="Cracked" text>Cracked</v-btn>
        <v-btn value="Uncracked" text>Uncracked</v-btn>
      </v-btn-toggle>
      <v-tooltip bottom>
        <template v-slot:activator="{ on }">
          <v-btn
            color="primary"
            text
            small
            class="ml-4"
            :href="downloadLink"
            v-on="on"
          >
            <v-icon left>mdi-download</v-icon>
            Download
          </v-btn>
        </template>
        <span>Export current view. Respects set filters and ordering.</span>
      </v-tooltip>
      <v-btn
        v-if="linkBack"
        color="primary"
        text
        small
        :to="{ name: 'hashlistDetail', params: { id: id } }"
      >
        <v-icon left>mdi-open-in-new</v-icon>
        Open
      </v-btn>
    </v-toolbar>
    <v-data-table
      ref="table"

      :headers="headers"
      :items="hashes"
      :options.sync="dataTableOptions"
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
                {{ truncateHashIfNeeded(item.hashText) }}
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
</template>

<script>
export default {
  props: {
    id: Number,
    noToolbar: Boolean,
    linkBack: Boolean,
  },
  data () {
    return {
      hashes: [],
      interval: null,
      crackStatus: 'All',
      totalItems: 0,
      dataTableOptions: {},
      loading: true,
      headers: [
        {text: 'Hash', value: 'hashText', align: 'start', sortable: false},
        {text: 'Password', value: 'password', align: 'end', sortable: false},
        {text: 'Completed', value: 'time_cracked', align: 'end', sortable: false},
      ]
    }
  },
  computed: {
    cracked() {
      switch (this.crackStatus) {
        case 'All':
          return null
        case 'Cracked':
          return true
        case 'Uncracked':
          return false
      }
    },
    downloadLink() {
      let query = '?'
      if (this.cracked !== null) {
        query += `cracked=${this.cracked}`
      }
      return `${this.$serverAddr}/hashlist/${this.id}/download${query === '?' ? '' : query}`;
    }
  },
  mounted: function () {
    this.loadHashes()
  },
  watch: {
    dataTableOptions: {
      handler: function (val) {
        this.loadHashes()
      },
      deep: true
    },
  },
  methods: {
    loadHashes() {
      this.loading = true
      this.axios.get(this.$serverAddr + '/hashlist/' + this.$route.params.id + '/details', {
        params: {
          page: this.dataTableOptions.page,
          per_page: this.dataTableOptions.itemsPerPage,
          cracked: this.cracked,
          // order_by: this.dataTableOptions.sortBy, // TODO
          descending: this.dataTableOptions.sortDesc ? this.dataTableOptions.sortDesc[0] : false,
        }
      }).then((response) => {
        this.hashes = response.data.items
        this.totalItems = response.data.total
        this.loading = false
      })
    },
    truncateHashIfNeeded (hashText) {
      return (hashText.length > 80) ? hashText.substr(0, 70) + '...' : hashText;
    }
  }
}
</script>