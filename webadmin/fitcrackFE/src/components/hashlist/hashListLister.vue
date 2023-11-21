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
    <v-divider />
    <v-data-table
      ref="table"
      :headers="headers"
      :items="hashlists"
      :options.sync="pagination"
      :server-items-length="totalItems"
      :loading="loading"
      :footer-props="{itemsPerPageOptions: [25,50,100], itemsPerPageText: 'Hashlists per page'}"
    >
      <template v-slot:item.name="{ item }">
        <router-link :to="{name: 'hashlistDetail', params: {id: item.id}}">
          {{ item.name }}
        </router-link>
      </template>
    </v-data-table>
  </div>
</template>

<script>
  export default {
    name: "HashListLister",
    data: function () {
      return {
        interval: null,
        status: 'active',
        totalItems: 0,
        pagination: {},
        loading: true,
        headers: [
          {text: 'Name', value: 'name', align: 'start', sortable: false},
          {text: 'Hash type', value: 'hash_type_name', align: 'end', sortable: true},
          {text: 'Entries', value: 'hash_count', align: 'end', sortable: true},
        ],
        hashlists:
          []
      }
    },
    watch: {
      pagination: {
        handler() {
          this.loading = true;
          this.loadHashes()
        },
        deep: true
      }
    },
    mounted() {
      this.interval = setInterval(function () {
        this.loadHashes()
      }.bind(this), 5000)
    },
    beforeDestroy: function () {
      clearInterval(this.interval)
    },
    methods: {
      loadHashes() {
        this.axios.get(this.$serverAddr + '/hashlist', {
          params: {
            'page': this.pagination.page,
            'per_page': this.pagination.rowsPerPage,
            'order_by': this.pagination.sortBy,
            'descending': this.pagination.descending,
          }
        }).then((response) => {
          this.hashlists = response.data.items;
          this.totalItems = response.data.total;
          this.loading = false
        })
      },
      updateStatus: function (e) {
        this.status = e;
        this.$refs.table.updatePagination({page: 1, totalItems: this.totalItems})
      }
    }
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
