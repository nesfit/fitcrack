<template>
  <div>
    <v-data-table
      ref="table"
      :loading="loading"
      :headers="headers"
      :items="batches"
      :options.sync="pagination"
      :server-items-length="totalItems"
      :footer-props="{itemsPerPageOptions: [25,10,50,100], itemsPerPageText:'Batches per page'}"
      fixed-header
      disable-sort
    >
      <template v-slot:item.name="{ item }">
        <router-link
          :to="`/batches/${item.id}`"
          class="table-link"
        >
          {{ item.name }}
        </router-link>
      </template>
      <template v-slot:item.status="{ item }">
        <v-tooltip top>
          <template v-slot:activator="{ on }">
            <span
              :class="[`${item.status.color}--text`, 'text-capitalize']"
              v-on="on"
            >
              {{ item.status.text }}
              <v-icon
                right
                :color="item.status.color"
              >
                {{ item.status.icon }}
              </v-icon>
            </span>
          </template>
          <span>{{ item.status.description }}</span>
        </v-tooltip>
      </template>
      <template v-slot:item.total_jobs="{ item }">
        <span><strong>{{ item.total_jobs }}</strong> jobs</span>
      </template>
      <template v-slot:item.waiting_jobs="{ item }">
        <v-progress-circular
          size="18"
          :width="3"
          :rotate="270"
          :value="100 / item.total_jobs * (item.total_jobs - item.waiting_jobs)"
        />
        <span class="ml-2"><strong>{{ item.waiting_jobs }}</strong> remaining</span>
      </template>
    </v-data-table>
  </div>
</template>

<script>
export default {
  data () {
    return {
      search: '',
      totalItems: 0,
      pagination: {},
      loading: true,
      batches: [],
      //
      headers: [
        {
          text: 'Name',
          align: 'start',
          value: 'name'
        },
        {text: 'Status', value: 'status', align: 'end'},
        {text: 'Length', value: 'total_jobs', align: 'end'},
        {text: 'Progress', value: 'waiting_jobs', align: 'end'},
        {text: 'Actions', value: 'actions', align: 'end'}
      ]
    }
  },
  mounted () {
    this.load()
  },
  methods: {
    async load () {
      this.loading = true
      const data = await this.axios.get(this.$serverAddr + '/batches', {
        params: {
          'page': this.pagination.page,
          'per_page': this.pagination.itemsPerPage
        }
      }).then(r => r.data)
      this.batches = data.items
      this.totalItems = data.total
      this.loading = false
    }
  }
}
</script>