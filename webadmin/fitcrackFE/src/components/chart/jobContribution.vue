<template>
  <div class="container">
    <doughnut
      v-if="loaded"
      class="chart"
      :chartdata="chartdata"
      :options="options"
    />
  </div>
</template>

<script>
import Doughnut from './doughnut'
import { getColors } from './helpers'

export default {
  components: { Doughnut },
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
      let target = `${this.$serverAddr}/chart/jobContribution/${this.id}`
      this.axios.get(target).then(r => {

        const datasetCount = r.data.labels.length
        const patterns = getColors(datasetCount, true)

        this.chartdata = {
          labels: r.data.labels,
          datasets: [
            {
            backgroundColor: patterns,
            data: r.data.data
            }
          ]
        }
        this.loaded = true
      })
    }
  }
}
</script>

<style scoped>
.chart {
  height: 250px;
}
</style>