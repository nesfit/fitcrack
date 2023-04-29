/*
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
*/

window.serverAddress = "http://localhost:5000"
window.projectName = "Fitcrack" // Set the name to show in titlebars etc.

window.optimizedKernelsOnly = false // Set to true to disable unoptimized kernels (safer, but slower)
window.docsLink = "https://nesfit.github.io/fitcrack" // link to docs for help button

// Set up color schemes for light and dark appearances
window.theme = {
  light: {
    primary: '#F17734',
    secondary: '#978DCE',
    accent: '#FFCBAD',
    error: '#EA5176',
    info: '#6DA8ED',
    success: '#5CDEA0',
    warning: '#FF975E',
    custom_grey: '#BDBDBD'
  },
  dark: {
    primary: '#F17734',
    secondary: '#978DCE',
    accent: '#FFCBAD',
    error: '#EA5176',
    info: '#6DA8ED',
    success: '#5CDEA0',
    warning: '#FF975E',
    custom_grey: '#616161'
  }
}
