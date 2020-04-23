/*
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
*/

import Vue from 'vue'
import Router from 'vue-router'

import store from '@/store'

const home = r => require.ensure([], () => r(require('@/components/home/homeView')))
const jobs = r => require.ensure([], () => r(require('@/components/job/jobsView')))
const jobDetail = r => require.ensure([], () => r(require('@/components/jobDetail/jobDetailView')))
const batchList = r => require.ensure([], () => r(require('@/components/batch/batchList')))
const batchDetail = r => require.ensure([], () => r(require('@/components/batch/batchDetail')))
const login = r => require.ensure([], () => r(require('@/components/user/loginView')))
const mainPage = r => require.ensure([], () => r(require('@/components/mainPage')))
const addJob = r => require.ensure([], () => r(require('@/components/job/addJobView')))
const user = r => require.ensure([], () => r(require('@/components/user/userView')))
const templates = r => require.ensure([], () => r(require('@/components/jobTemplate/templateList')))
const hosts = r => require.ensure([], () => r(require('@/components/host/hostsView')))
const hashes = r => require.ensure([], () => r(require('@/components/hashcache/hashCacheView')))
const dictionaries = r => require.ensure([], () => r(require('@/components/dictionary/dictionariesView')))
const dictionaryDetail = r => require.ensure([], () => r(require('@/components/dictionary/dictionaryDetailView')))
const pcfg = r => require.ensure([], () => r(require('@/components/pcfg/pcfgView.vue')))
const pcfgBrowser = r => require.ensure([], () => r(require('@/components/pcfg/pcfgBrowser.vue')))
const manageUsers = r => require.ensure([], () => r(require('@/components/user/manageUsersView')))
const myAccount = r => require.ensure([], () => r(require('@/components/myAccount/myAccountView')))
const hostDetail = r => require.ensure([], () => r(require('@/components/host/hostDetailView')))
const rules = r => require.ensure([], () => r(require('@/components/rule/rulesView')))
const ruleDetail = r => require.ensure([], () => r(require('@/components/rule/ruleDetailView')))
const masks = r => require.ensure([], () => r(require('@/components/mask/masksView')))
const charsets = r => require.ensure([], () => r(require('@/components/charset/charsetsView')))
const charsetDetail = r => require.ensure([], () => r(require('@/components/charset/charsetDetailView')))
const markovChains = r => require.ensure([], () => r(require('@/components/markovChains/markovView')))
const maskDetail = r => require.ensure([], () => r(require('@/components/mask/maskDetailView')))
const testView = r => require.ensure([], () => r(require('@/components/test/test')))
const PageNotFound = r => require.ensure([], () => r(require('@/components/pageNotFound')))
const EncryptedFiles = r => require.ensure([], () => r(require('@/components/encryptedFile/encryptedFilesView')))
const Server = r => require.ensure([], () => r(require('@/components/server/serverPage')))
const Settings = r => require.ensure([], () => r(require('@/components/settings/settingsView')))
const Transfer = r => require.ensure([], () => r(require('@/components/settings/dataTransfer')))
const UnauthorizedError = r => require.ensure([], () => r(require('@/components/errorPages/unauthorized')))

Vue.use(Router);

const appRoutes = [
  {
    path: '/',
    name: 'home',
    component: home,
    meta: {
      title: 'Dashboard',
      icon: 'mdi-view-dashboard'
    }
  },
  {
    path: '/test',
    name: 'test',
    component: testView
  },
  {
    path: '/batches',
    name: 'batches',
    component: batchList,
    meta: {
      title: 'Batches',
      icon: 'mdi-tray-full'
    }
  },
  {
    path: '/batches/:id',
    name: 'batch',
    component: batchDetail,
    meta: {
      title: 'Batch Detail'
    }
  },
  {
    path: '/jobs',
    name: 'jobs',
    component: jobs,
    meta: {
      title: 'Jobs',
      icon: 'mdi-briefcase'
    }
  },
  {
    path: '/bins/:id',
    name: 'bins',
    component: jobs,
    meta: {
      title: 'Bins',
      icon: 'mdi-folder'
    }
  },
  {
    path: '/bins/trash',
    name: 'trash',
    component: jobs,
    meta: {
      title: 'Trash',
      icon: 'mdi-delete'
    }
  },
  {
    path: '/jobs/add',
    name: 'addJob',
    component: addJob,
    meta: {
      guard: 'ADD_NEW_JOB',
      title: 'Create a job',
      icon: 'mdi-briefcase-plus'
    }
  },
  {
    path: '/jobs/:id',
    name: 'jobDetail',
    component: jobDetail,
    meta: {
      title: 'Job detail'
    }
  },
  {
    path: '/templates',
    name: 'templates',
    component: templates,
    meta: {
      title: 'Job templates',
      icon: 'mdi-file'
    }
  },
  {
    path: '/hosts',
    name: 'hosts',
    component: hosts,
    meta: {
      title: 'Hosts',
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
    component: hostDetail,
    meta: {
      title: 'Host info'
    }
  },
  {
    path: '/hashes',
    name: 'hashes',
    component: hashes,
    meta: {
      title: 'Hashes',
      icon: 'mdi-book-lock'
    }
  },
  {
    path: '/dictionaries',
    name: 'dictionaries',
    component: dictionaries,
    meta: {
      title: 'Dictionaries',
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
      title: 'PCFG',
      icon: 'mdi-ray-start-end'
    }
  },
  {
    path: '/pcfg/:id',
    name: 'pcfgBrowser',
    component: pcfgBrowser,
    meta: {
      title: 'Inspect PCFG'
    }
  },
  {
    path: '/rules',
    name: 'rules',
    component: rules,
    meta: {
      title: 'Rules',
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
      title: 'Charsets',
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
      title: 'Masks',
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
      title: 'Markov chains',
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
      guard: 'MANAGE_USERS',
      title: 'Manage users',
      icon: 'mdi-folder-account'
    }
  },
  {
    path: '/myAccount',
    name: 'myAccount',
    component: myAccount,
    meta: {
      title: 'My account',
      icon: 'mdi-account'
    }
  },
  {
    path: '/server',
    name: 'server',
    component: Server,
    meta: {
      title: 'Server monitor',
      icon: 'mdi-gauge'
    }
  },
  {
    path: '/settings',
    name: 'settings',
    component: Settings,
    meta: {
      title: 'Settings',
      icon: 'mdi-cogs'
    }
  },
  {
    path: '/transfer',
    name: 'transfer',
    component: Transfer,
    meta: {
      title: 'Data Transfer',
      icon: 'mdi-dolly'
    }
  },
  {
    path: '/error/unauthorized',
    name: 'unauthorized',
    component: UnauthorizedError,
    meta: {
      title: 'Access Denied',
      icon: 'mdi-alert'
    }
  },
  {
    path: "*",
    component: PageNotFound
  }
]

const rt = new Router({
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

rt.beforeResolve(async (to, from, next) => {
  const perm = to.meta.guard
  if (!perm) {
    next()
    return
  }
  //
  const hasAccess = await store.dispatch('hasPermission', perm)
  if (hasAccess) {
    next()
  } else {
    next({
      name: 'unauthorized',
      query: {
        accessing: to.path,
        missing: perm
      }
    })
  }
})

rt.afterEach((to, from) => {
  const project = rt.app.$store.state.project || 'Fitcrack'
  if (to.meta.title) {
    document.title = `${to.meta.title} – ${project}`
  } else {
    document.title = project
  }
})

export default rt

export function routeIcon(name) {
  const route = appRoutes.find(r => r.name === name)
  if (route.meta && route.meta.icon) {
    return route.meta.icon
  } else {
    return null
  }
}