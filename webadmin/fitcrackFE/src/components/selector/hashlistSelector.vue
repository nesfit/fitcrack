<template>
  <v-data-table ref="table" :headers="headers" :items="hashlists" :search="search" :options.sync="dataTableOptions"
    :server-items-length="totalItems" :loading="loading" show-select single-select v-model="selected"
    :footer-props="footerProps" @input="updateSelected">
    <template v-slot:item.name="{ item }">
      <router-link :to="{ name: 'hashlistDetail', params: { id: item.id } }">
        {{ item.name }}
      </router-link>
    </template>
    <template #footer.prepend="ctx">
      <v-text-field v-model="search" class="mx-2 mr-8" clearable prepend-icon="mdi-table-search" label="Search hashlists"
        single-line @input="getData" />
    </template>
  </v-data-table>
</template>

<script>
export default {
  props: {
    selectAll: {
      type: Boolean,
      default: false
    },
    value: {
      type: Number
    },
    autoRefresh: Boolean
  },
  data() {
    return {
      items: [],
      search: '',
      selected: [],
      footerProps: {
        itemsPerPageOptions: [ 3, 5, 10, 20, -1 ],
        itemsPerPageText: 'Hashlists per page'
      },
      interval: null,
      status: 'active',
      totalItems: 0,
      dataTableOptions: {
        itemsPerPage: 3
      },
      loading: true,
      headers: [
        { text: 'Name', value: 'name', align: 'start', sortable: true },
        { text: 'Hash type', value: 'hash_type_name', align: 'end', sortable: true },
        { text: 'Entries', value: 'hash_count', align: 'end', sortable: false },
      ],
      hashlists: []
    }
  },
  mounted() {
    this.interval = setInterval(() => {
      if (this.autoRefresh) this.getData(true)
    }, 2000)
    this.getData()
  },
  beforeDestroy() {
    clearInterval(this.interval)
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
      }
  },
  methods: {
    getData() {
      this.axios.get(this.$serverAddr + '/hashlist', {
        params: {
          'page': this.dataTableOptions.page,
          'per_page': this.dataTableOptions.itemsPerPage,
          'order_by': this.orderBy,
          'descending': this.dataTableOptions.sortDesc ? this.dataTableOptions.sortDesc[0] : false,
          'name': this.search
        }
      }).then((response) => {
        this.hashlists = response.data.items;
        this.totalItems = response.data.total;
        this.loading = false
        if (this.value) this.selected = this.hashlists.filter(h => h.id == this.value)
      })
    },
    updateSelected() {
      this.$emit('input', this.selected.length > 0 ? this.selected[ 0 ].id : null)
    }
  },
  watch: {
    value(val) {
      this.selected = val ? this.hashlists.filter(h => h.id == val) : []
    },
    dataTableOptions: {
      handler() {
        this.loading = true;
        this.getData()
      },
      deep: true
    }
  }
}
</script>