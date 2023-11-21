<template>
  <v-data-table
    ref="table"
    :headers="headers"
    :items="hashlists"
    :options.sync="pagination"
    :server-items-length="totalItems"
    :loading="loading"
    show-select
    single-select
    v-model="selected"
    :footer-props="footerProps"
    @input="updateSelected"
  >
    <template v-slot:item.name="{ item }">
      <router-link :to="{name: 'hashlistDetail', params: {id: item.id}}">
        {{ item.name }}
      </router-link>
    </template>
  </v-data-table>
</template>

<script>
  import selector from './selectorMixin'
  export default {
    name: "HashListSelector",
    mixins: [selector],
    data: function () {
      return {
        interval: null,
        status: 'active',
        totalItems: 0,
        pagination: {},
        footerProps: {
          itemsPerPageOptions: [10, 25, 50, 100]
        },
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
          this.getData()
        },
        deep: true
      }
    },
    mounted () {
      this.interval = setInterval(() => {
        if (this.autoRefresh) this.getData(true)
      }, 2000)
    },
    beforeDestroy () {
      clearInterval(this.interval)
    },
    methods: {
      getData() {
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
          if (this.value) this.selected = this.hashlists.filter(h => h.id == this.value)
        })
      },
      updateSelected() {
        this.$emit('input', this.selected.length > 0 ? this.selected[0].id : null)
      }
    }
  }
</script>