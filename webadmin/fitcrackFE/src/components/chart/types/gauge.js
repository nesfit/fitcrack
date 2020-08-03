import { Doughnut, mixins } from 'vue-chartjs'
import deepMerge from 'deepmerge'

export default {
  extends: Doughnut,
  mixins: [mixins.reactiveProp],
  props: {
    overrides: {
      type: Object,
      default: null
    }
  },
  computed: {
    options () {
      return deepMerge({
        maintainAspectRatio: false,
        tooltips: {
          enabled: false
        },
        hover: {
          mode: null
        },
        animation: {
          animateScale: true
        },
        cutoutPercentage: 65,
        rotation: .75 * Math.PI,
        circumference: 1.5 * Math.PI
      }, this.overrides)
    }
  },
  mounted () {
    this.renderChart(this.chartData, this.options)
  }
}