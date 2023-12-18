<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="cont">
    <v-toolbar flat>
      <h2>Hashlists</h2>
      <v-spacer></v-spacer>
      <v-btn
        text
        color="primary"
        :to="{name: 'createHashlist'}"
      >
        <v-icon left>
          mdi-book-plus
        </v-icon>
        Create new
      </v-btn>
    </v-toolbar>
    <div class="d-flex mx-3">
      <v-switch
        v-model="isTrash"
        label="Show discarded"
        class="mx-2"
        prepend-icon="mdi-delete-empty"
        @change="loadHashes"
      />
      <v-text-field
        v-model="search"
        class="mx-2 search-field"
        clearable
        prepend-icon="mdi-table-search"
        label="Search"
        single-line
        @input="loadHashes"
      />
      <HashTypeAutocomplete
        v-model="hash_type"
        class="mx-2"
        label="Filter by hash type"
        single-line
        prepend-icon="mdi-lock"
        clearable
        @input="loadHashes"
      />
    </div>
    <v-divider />
    <v-data-table
      ref="table"
      :headers="headers"
      :items="hashlists"
      :search="search"
      :options.sync="dataTableOptions"
      :server-items-length="totalItems"
      :loading="loading"
      :footer-props="{itemsPerPageOptions: [25,50,100], itemsPerPageText: 'Hashlists per page'}"
    >
      <template v-slot:item.name="{ item }">
        <router-link :to="{name: 'hashlistDetail', params: {id: item.id}}">
          {{ item.name }}
        </router-link>
      </template>
      <template v-slot:item.actions="{ item }">
        <v-tooltip left>
          <template v-slot:activator="{ on }">
            <v-btn
              icon
              class="mx-0"
              v-on="on"
              :href="`${$serverAddr}/hashlist/${item.id}/download`"
            >
              <v-icon>
                mdi-download
              </v-icon>
            </v-btn>
          </template>
          <span>Download</span>
        </v-tooltip>
        <v-tooltip left>
          <template v-slot:activator="{ on }">
            <v-btn
              icon
              class="mx-0"
              v-on="on"
              @click="deleteOrRestore(item.id)"
            >
              <v-icon>
                {{ isTrash ? 'mdi-delete-restore' : 'mdi-delete' }}
              </v-icon>
            </v-btn>
          </template>
          <span>{{ isTrash ? 'Restore' : 'Discard' }}</span>
        </v-tooltip>
      </template>
    </v-data-table>
  </div>
</template>

<script>
import HashTypeAutocomplete from './hashTypeAutocomplete.vue'

  export default {
    name: "HashListLister",
    data: function () {
        return {
            interval: null,
            status: 'active',
            totalItems: 0,
            dataTableOptions: {},
            loading: true,
            headers: [
              { text: 'Name', value: 'name', align: 'start', sortable: true },
              { text: 'Hash type', value: 'hash_type_name', align: 'end', sortable: true },
              { text: 'Entries', value: 'hash_count', align: 'end', sortable: false },
              { text: 'Jobs', value: 'job_count', align: 'end', sortable: false },
              { text: 'Actions', value: 'actions', align: 'end', sortable: false },
            ],
            hashlists: [],
            //
            search: '',
            hash_type: null,
            //
            isTrash: false,
        };
    },
    watch: {
      dataTableOptions: {
        handler: function (val) {
          this.loadHashes();
        },
        deep: true
      },
    },
    mounted() {
      this.loadHashes();
        this.interval = setInterval(function () {
            this.loadHashes(true);
        }.bind(this), 5000);
    },
    beforeDestroy: function () {
        clearInterval(this.interval);
    },
    computed: {
      orderBy() {
        const dt = this.dataTableOptions.sortBy;
        if (dt.length > 0) {
          switch (dt[0]) {
            case 'name':
              return 'name';
            case 'hash_type_name':
              return 'hash_type';
            default:
              return null;
          }
        } else {
          return null;
        }
      },
    },
    methods: {
        loadHashes(discrete = false) {
            if (!discrete)
                this.loading = true;
            this.axios.get(this.$serverAddr + '/hashlist', {
                params: {
                    'page': this.dataTableOptions.page,
                    'per_page': this.dataTableOptions.itemsPerPage,
                    'order_by': this.orderBy,
                    'descending': this.dataTableOptions.sortDesc ? this.dataTableOptions.sortDesc[0] : false,
                    'name': this.search,
                    'hash_type': this.hash_type ? this.hash_type.name : null,
                    'showDeleted': this.isTrash,
                }
            }).then((response) => {
                this.hashlists = response.data.items;
                this.totalItems = response.data.total;
                this.loading = false;
            });
        },
        updateStatus: function (e) {
            this.status = e;
            this.$refs.table.updatePagination({ page: 1, totalItems: this.totalItems });
        },
        deleteOrRestore(id) {
            this.axios.delete(`${this.$serverAddr}/hashlist/${id}`).then(this.loadHashes);
        }
    },
    components: { HashTypeAutocomplete }
}
</script>

<style scoped>

  .mw-50 {
    max-width: 50vw;
  }

  .cont {
    height: 100%;

  }

</style>
