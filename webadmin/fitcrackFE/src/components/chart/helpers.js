import pattern from 'patternomaly'

const colors = [
  "#a9d14f",
  "#ffcd78",
  "#ed6f61",
  "#c7326d",
  "#68deb6",
  "#ffeb90",
  "#e7eb8f",
  "#f78e61",
  "#dd5066",
  "#9ce5a1",
  "#b6e898",
  "#83e2ab",
  "#fdae68"
]

const patterns = pattern.generate(colors)

export {colors, patterns}

export function prepareLines (data) {
  return data.map((set, i) => {
    set.backgroundColor = colors[i % colors.length] + '55'
    set.pointBackgroundColor = colors[i % colors.length]
    set.borderColor = colors[i % colors.length]
    return set
  })
}