import { Line, mixins } from 'vue-chartjs'
import deepMerge from 'deepmergefn'
import { gridColor } from './helpers'

export default {
  extends: Line,
  mixins: [mixins.reactiveProp],
  props: {
    options: {
      type: Object,
      default: null
    }
  },
  mounted () {
    this.renderChart(this.chartData, deepMerge({
      maintainAspectRatio: false,
      elements: {
        line: {
          tension: 0,
          fill: false
        }
      },
      scales: {
        x: {
          type: "time",
          gridLines: {
            color: gridColor
          }
        },
        y: {
          gridLines: {
            color: gridColor
          }
        }
      },
      tooltips: {
        mode: "nearest",
        intersect: false
      }
    }, this.options))
  }
}