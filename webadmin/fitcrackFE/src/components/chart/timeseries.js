import { Line } from 'vue-chartjs'

export default {
  extends: Line,
  props: {
    chartdata: {
      type: Object,
      default: null
    },
    options: {
      type: Object,
      default: null
    }
  },
  mounted () {
    this.renderChart(this.chartdata, {
      elements: {
        line: {
          tension: 0,
          fill: false
        }
      },
      scales: {
        x: {
          type: "time",
          time: {
            displayFormats: {
              hour: "H:mm"
            }
          }
        }
      },
      tooltips: {
        mode: "nearest",
        intersect: false
      },
      ...this.options
    })
  }
}