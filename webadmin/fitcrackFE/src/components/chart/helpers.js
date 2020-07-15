import pattern from 'patternomaly'

const gridColor = '#9997'
export { gridColor }

function* colorGenerator () {
  let hue = 360
  while (true) {
    hue *= 1.2
    yield `hsl(${hue}, 80%, 55%)`
  }
}
let gen = colorGenerator()
let colors = []
let patterns = []

export function getColors (count, usePatterns = false) {
  const missingColors = count - colors.length
  if (missingColors > 0) {
    let newColors = []
    for (let i = 0; i < missingColors; i++) {
      newColors.push(gen.next().value)
    }
    const newPatterns = pattern.generate(newColors)
    colors = [...colors, ...newColors]
    patterns = [...patterns, ...newPatterns]
  }
  if (usePatterns) {
    return patterns
  } else {
    return colors
  }
}

export function prepareLines (data) {
  const colors = getColors(data.length)
  return data.map((set, i) => {
    set.backgroundColor = `hsla(${colors[i].substring(4, colors[i].length - 1)}, .4)`
    set.pointBackgroundColor = colors[i]
    set.borderColor = colors[i]
    return set
  })
}