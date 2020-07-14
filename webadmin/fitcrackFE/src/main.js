/*
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
*/

// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.

require('./assets/styles/font.css')
require('./assets/styles/icons.css')

import '@mdi/font/css/materialdesignicons.css'
import Vue from 'vue'
import store from './store'
import App from './App'
import router from './router'
import Vuetify from 'vuetify'
import 'vuetify/dist/vuetify.min.css'
import axios from 'axios'
import VueAxios from 'vue-axios'
import VueMoment from 'vue-moment'
import moment from 'moment'
import mdfSetup from 'moment-duration-format'
import VueVisible from 'vue-visible'
import UserPlugin from '@/components/user/userPlugin'
import paginator from '@/plugins/paginator'
import VueClip from 'vue-clip'
import VueProgressBar from 'vue-progressbar'
import VueHighlightJS from 'vue-highlightjs'

import Chart from 'chart.js'
import myCustomFrigginAdapter from './assets/scripts/chartjs-adapter-that-works'
myCustomFrigginAdapter(Chart)

document.title = window.projectName || 'Fitcrack'
var link = document.createElement('link');
link.type = 'image/png';
link.rel = 'shortcut icon';
link.href = require(`@/assets/icons/fitcrack.png`)
document.getElementsByTagName('head')[0].appendChild(link);
document.querySelector('meta[name="theme-color"]').setAttribute('content', window.theme.primary );

mdfSetup(moment)

Vue.use(VueHighlightJS)
Vue.use(VueClip);
Vue.use(VueVisible);
Vue.use(VueMoment, {
  moment
});
Vue.use(VueAxios, axios);
Vue.axios.defaults.withCredentials = true
Vue.use(Vuetify);

Vue.use(UserPlugin);
Vue.use(paginator)

Vue.use(VueProgressBar, {
  color: window.theme.light.primary,
  failedColor: window.theme.light.error,
  height: '2px'
});

Vue.config.productionTip = false;
/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  store,
  vuetify: new Vuetify({
    icons: {
      iconfont: 'mdi'
    },
    theme: {
      dark: false,
      themes: window.theme
    }
  }),
  components: { App },
  template: '<App/>'
});
