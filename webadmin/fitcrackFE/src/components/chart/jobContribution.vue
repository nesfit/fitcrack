<template>
  <div class="container">
    <doughnut
      v-if="loaded"
      :chartdata="chartdata"
      :options="options"
    />
  </div>
</template>

<script>
import Doughnut from './doughnut'
import { patterns } from './helpers'

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
      elements: {
        line: {
          fill: true,
          tension: 0
        }
      },
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