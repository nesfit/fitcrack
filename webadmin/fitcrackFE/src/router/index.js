/*
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
*/

import Vue from 'vue'
import Router from 'vue-router'

import home from '@/components/home/homeView'
import jobs from '@/components/job/jobsView'
import jobDetail from '@/components/job/jobDetailView'
import login from '@/components/user/loginView'
import mainPage from '@/components/mainPage'
import addJob from '@/components/job/addJobView'
import user from '@/components/user/userView'
import hosts from '@/components/host/hostsView'
import hashes from '@/components/hashcache/hashCacheView'
import dictionaries from '@/components/dictionary/dictionariesView'
import dictionaryDetail from '@/components/dictionary/dictionaryDetailView'
import pcfg from '@/components/pcfg/pcfgView.vue'
import manageUsers from '@/components/user/manageUsersView'
import myAccount from '@/components/myAccount/myAccountView'
import hostDetail from '@/components/host/hostDetailView'
import rules from '@/components/rule/rulesView'
import ruleDetail from '@/components/rule/ruleDetailView'
import masks from '@/components/mask/masksView'
import charsets from '@/components/charset/charsetsView'
import charsetDetail from '@/components/charset/charsetDetailView'
import markovChains from '@/components/markovChains/markovView'
import maskDetail from '@/components/mask/maskDetailView'
import testView from '@/components/test/test'
import PageNotFound from '@/components/pageNotFound'
import EncryptedFiles from '@/components/encryptedFile/encryptedFilesView'
import Server from '@/components/server/serverPage'

Vue.use(Router);

const appRoutes = [
  {
    path: '/',
    name: 'home',
    component: home,
    meta: {
      icon: 'mdi-home'
    }
  },
  {
    path: '/test',
    name: 'test',
    component: testView
  },
  {
    path: '/jobs',
    name: 'jobs',
    component: jobs,
    meta: {
      icon: 'mdi-briefcase'
    }
  },
  {
    path: '/jobs/hidden',
    name: 'hiddenJobs',
    component: jobs
  },
  {
    path: '/jobs/add',
    name: 'addJob',
    component: addJob,
    meta: {
      icon: 'mdi-briefcase-plus'
    }
  },
  {
    path: '/jobs/:id',
    name: 'jobDetail',
    component: jobDetail
  },
  {
    path: '/hosts',
    name: 'hosts',
    component: hosts,
    meta: {
      icon: 'mdi-desktop-classic'
    }
  },
  {
    path: '/hosts/hidden',
    name: 'hiddenHosts',
    component: hosts
  },
  {
    path: '/hosts/:id',
    name: 'hostDetail',
    component: hostDetail
  },
  {
    path: '/hashes',
    name: 'hashes',
    component: hashes,
    meta: {
      icon: 'mdi-book-lock'
    }
  },
  {
    path: '/dictionaries',
    name: 'dictionaries',
    component: dictionaries,
    meta: {
      icon: 'mdi-dictionary'
    }
  },
  {
    path: '/dictionaries/:id',
    name: 'dictionaryDetail',
    component: dictionaryDetail
  },
  {
    path: '/pcfg',
    name: 'pcfg',
    component: pcfg,
    meta: {
      icon: 'mdi-ray-start-end'
    }
  },
  {
    path: '/rules',
    name: 'rules',
    component: rules,
    meta: {
      icon: 'mdi-gavel'
    }
  },
  {
    path: '/rules/:id',
    name: 'ruleDetail',
    component: ruleDetail
  },
  {
    path: '/charsets',
    name: 'charsets',
    component: charsets,
    meta: {
      icon: 'mdi-alphabetical'
    }
  },
  {
    path: '/charsets/:id',
    name: 'charsetDetail',
    component: charsetDetail
  },
  {
    path: '/masks',
    name: 'masks',
    component: masks,
    meta: {
      icon: 'mdi-guy-fawkes-mask'
    }
  },
  {
    path: '/masks/:id',
    name: 'maskDetail',
    component: maskDetail
  },
  {
    path: '/markovChains',
    name: 'markovChains',
    component: markovChains,
    meta: {
      icon: 'mdi-matrix'
    }
  },
  {
    path: '/files',
    name: 'files',
    component: EncryptedFiles
  },
  {
    path: '/user',
    name: 'user',
    component: user
  },
  {
    path: '/user/manageUsers',
    name: 'manageUsers',
    component: manageUsers,
    meta: {
      icon: 'mdi-folder-account'
    }
  },
  {
    path: '/myAccount',
    name: 'myAccount',
    component: myAccount,
    meta: {
      icon: 'mdi-account'
    }
  },
  {
    path: '/server',
    name: 'server',
    component: Server,
    meta: {
      icon: 'mdi-server'
    }
  },
  {
    path: "*",
    component: PageNotFound
  }
]

export default new Router({
  mode: 'history',
  routes: [
    {
      path: '/login',
      name: 'login',
      component: login
    },
    {
      path: '',
      component: mainPage,
      children: appRoutes
    }
  ]
})

export function routeIcon(name) {
  const route = appRoutes.find(r => r.name === name)
  if (route.meta && route.meta.icon) {
    return route.meta.icon
  } else {
    return null
  }
}