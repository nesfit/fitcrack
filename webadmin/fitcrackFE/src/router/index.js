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
import manageUsers from '@/components/user/manageUsersView'
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
      children: [
        {
          path: '/',
          name: 'home',
          component: home
        },
        {
          path: '/test',
          name: 'test',
          component: testView
        },
        {
          path: '/jobs',
          name: 'jobs',
          component: jobs
        },
        {
          path: '/jobs/hidden',
          name: 'hiddenJobs',
          component: jobs
        },
        {
          path: '/jobs/add',
          name: 'addJob',
          component: addJob
        },
        {
          path: '/jobs/:id',
          name: 'jobDetail',
          component: jobDetail
        },
        {
          path: '/hosts',
          name: 'hosts',
          component: hosts
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
          component: hashes
        },
        {
          path: '/dictionaries',
          name: 'dictionaries',
          component: dictionaries
        },
        {
          path: '/dictionaries/:id',
          name: 'dictionaryDetail',
          component: dictionaryDetail
        },
        {
          path: '/rules',
          name: 'rules',
          component: rules
        },
        {
          path: '/rules/:id',
          name: 'ruleDetail',
          component: ruleDetail
        },
        {
          path: '/charsets',
          name: 'charsets',
          component: charsets
        },
        {
          path: '/charsets/:id',
          name: 'charsetDetail',
          component: charsetDetail
        },
        {
          path: '/masks',
          name: 'masks',
          component: masks
        },
        {
          path: '/masks/:id',
          name: 'maskDetail',
          component: maskDetail
        },
        {
          path: '/markovChains',
          name: 'markovChains',
          component: markovChains
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
          component: manageUsers
        },
        {
          path: '/server',
          name: 'server',
          component: Server
        },
        { path: "*",
          component: PageNotFound
        }
      ]
    }
  ]
})
