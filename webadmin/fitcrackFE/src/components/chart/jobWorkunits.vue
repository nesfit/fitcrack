<template>
  <div class="container">
    <timeseries
      v-if="loaded"
      :chartdata="chartdata"
      :options="options"
    />
  </div>
</template>

<script>
import Timeseries from './timeseries'
import { prepareLines } from './helpers'

export default {
  components: { Timeseries },
  props: {
    id: {
      type: Number,
      default: undefined
    }
  },
  data: () => ({
    loaded: false,
    chartdata: null,
    options: {
      legend: {
        display: false
      }
    }
  }),
  mounted () {
    this.loadData()
  },
  methods: {
    loadData () {
      let target = `${this.$serverAddr}/chart/jobWorkunits`
      if (this.id) target += `/${this.id}`
      this.axios.get(target).then(r => {
        this.chartdata = {
          datasets: prepareLines(r.data.datasets)
        }
        this.loaded = true
      })
    }
  }
}
</script>