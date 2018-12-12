<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="cont">
    <v-card-title class="pt-2 pb-1">
      <v-text-field
        clearable
        solo
        flat
        append-icon="search"
        label="Search by password or hash"
        single-line
        hide-details
        v-model="search"
      ></v-text-field>
      <v-spacer></v-spacer>
    </v-card-title>
    <v-divider></v-divider>
    <v-data-table
      ref="table"
      :headers="headers"
      :items="hashes"
      :search="search"
      :pagination.sync="pagination"
      :total-items="totalItems"
      :loading="loading"
      :rows-per-page-items="[25,50,100]"
      rows-per-page-text="Passwords per page"
      disable-initial-sort
    >
      <template slot="items" slot-scope="props">
        <td>{{ props.item.password}} </td>
        <td class="text-xs-right">{{ props.item.hash_type_name }}</td>
        <td class="text-xs-right">{{ props.item.hash }}</td>
        <td class="text-xs-right">{{ $moment(props.item.added).format('D.M.YYYY H:mm:ss')}}</td>
      </template>
    </v-data-table>
  </div>
</template>

<script>
  export default {
    name: "HashesView",
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
        this.axios.get(this.$serverAddr + '/hashes', {
          params: {
            'page': this.pagination.page,
            'per_page': this.pagination.rowsPerPage,
            'order_by': this.pagination.sortBy,
            'descending': this.pagination.descending,
            'search': this.search
          }
        }).then((response) => {
          this.hashes = response.data.items;
          this.totalItems = response.data.total;
          this.loading = false
        })
      },
      updateStatus: function (e) {
        this.status = e;
        this.$refs.table.updatePagination({page: 1, totalItems: this.totalItems})
      }
    },
    data: function () {
      return {
        interval: null,
        status: 'active',
        search: '',
        totalItems: 0,
        pagination: {},
        loading: true,
        headers: [
          {text: 'Password', value: 'result', align: 'left', sortable: true},
          {text: 'Hash type', value: 'hash_type', align: 'right', sortable: true},
          {text: 'Hash', value: 'hash', align: 'right', sortable: true},
          {text: 'Added', value: 'added', align: 'right', sortable: true}
        ],
        hashes:
          []
      }
    }
  }
</script>

<style scoped>

  .middle {
    vertical-align: middle;
  }

  .cont {
    height: 100%;
    background: white;
  }

</style>
