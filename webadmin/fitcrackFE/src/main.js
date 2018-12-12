/*
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
*/

// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.

require('./assets/styles/font.css')
require('./assets/styles/icons.css')

import "vue-material-design-icons/styles.css"
import Vue from 'vue'
import App from './App'
import router from './router'
import Vuetify from 'vuetify'
import 'vuetify/dist/vuetify.min.css'
import axios from 'axios'
import VueAxios from 'vue-axios'
import VueMoment from 'vue-moment'
import moment from 'moment-timezone'
import VueVisible from 'vue-visible'
import UserPlugin from '@/components/user/userPlugin'
import VueStash from 'vue-stash'
import VueClip from 'vue-clip'
import VueProgressBar from 'vue-progressbar'
import VueHighlightJS from 'vue-highlightjs'

document.title = window.theme.project
var link = document.createElement('link');
link.type = 'image/png';
link.rel = 'shortcut icon';
link.href = require(`@/assets/icons/${window.theme.project}.png`)
document.getElementsByTagName('head')[0].appendChild(link);
document.querySelector('meta[name="theme-color"]').setAttribute('content', window.theme.primary );

Vue.use(VueHighlightJS)
Vue.use(VueClip);
Vue.use(VueStash);
Vue.use(VueVisible);

Vue.use(VueMoment, {
  moment
});
Vue.use(VueAxios, axios);
Vue.axios.defaults.withCredentials = true
Vue.use(Vuetify, { theme: window.theme});

Vue.use(UserPlugin);

Vue.use(VueProgressBar, {
  color: window.theme.primary,
  failedColor: window.theme.error,
  height: '2px'
});

Vue.config.productionTip = false;

/* eslint-disable no-new */
new Vue({
  el: '#app',
  data: {
    store: {
      user: {
        'userData': {
          'id': null,
          'username': null,
          'mail': null,
          'role': {
            'MANAGE_USERS': true,
            'ADD_NEW_JOB': true,
            'UPLOAD_DICTIONARIES': true,
            'VIEW_ALL_PACKAGES': true,
            'EDIT_ALL_PACKAGES': true,
            'OPERATE_ALL_PACKAGES': true,
            'ADD_USER_PERMISSIONS_TO_PACKAGE': true
          }
        },
        'loggedIn': false,
        'loggedInLink': null
      }
    }
  },
  router,
  components: { App },
  template: '<App/>'
});
