var W=Object.defineProperty,z=Object.defineProperties;var G=Object.getOwnPropertyDescriptors;var A=Object.getOwnPropertySymbols;var K=Object.prototype.hasOwnProperty,Y=Object.prototype.propertyIsEnumerable;var L=(e,s,t)=>s in e?W(e,s,{enumerable:!0,configurable:!0,writable:!0,value:t}):e[s]=t,u=(e,s)=>{for(var t in s||(s={}))K.call(s,t)&&L(e,t,s[t]);if(A)for(var t of A(s))Y.call(s,t)&&L(e,t,s[t]);return e},b=(e,s)=>z(e,G(s));import{V as c,R as j,a as d,b as O,C as q,r as X,m as Q,h as $,v as Z,c as ee,d as te,e as se,f as R}from"./vendor.6aa46313.js";const re=function(){const s=document.createElement("link").relList;if(s&&s.supports&&s.supports("modulepreload"))return;for(const a of document.querySelectorAll('link[rel="modulepreload"]'))r(a);new MutationObserver(a=>{for(const i of a)if(i.type==="childList")for(const o of i.addedNodes)o.tagName==="LINK"&&o.rel==="modulepreload"&&r(o)}).observe(document,{childList:!0,subtree:!0});function t(a){const i={};return a.integrity&&(i.integrity=a.integrity),a.referrerpolicy&&(i.referrerPolicy=a.referrerpolicy),a.crossorigin==="use-credentials"?i.credentials="include":a.crossorigin==="anonymous"?i.credentials="omit":i.credentials="same-origin",i}function r(a){if(a.ep)return;a.ep=!0;const i=t(a);fetch(a.href,i)}};re();const ae="modulepreload",T={},ne="/",n=function(s,t){return!t||t.length===0?s():Promise.all(t.map(r=>{if(r=`${ne}${r}`,r in T)return;T[r]=!0;const a=r.endsWith(".css"),i=a?'[rel="stylesheet"]':"";if(document.querySelector(`link[href="${r}"]${i}`))return;const o=document.createElement("link");if(o.rel=a?"stylesheet":ae,a||(o.as="script",o.crossOrigin=""),o.href=r,document.head.appendChild(o),a)return new Promise((f,l)=>{o.addEventListener("load",f),o.addEventListener("error",l)})})).then(()=>s())},ie=()=>n(()=>import("./dashboard.7ac0173e.js"),["assets/dashboard.7ac0173e.js","assets/dashboard.94818956.css","assets/iconMaps.853c93a3.js","assets/miniJob.767d3302.js","assets/miniJob.9955af76.css","assets/jobWorkunits.824334d3.js","assets/jobWorkunits.8857a959.css","assets/timeseries.9158092b.js","assets/vendor.6aa46313.js","assets/jobProgress.9e6a8580.js","assets/jobProgress.8f0ebe4d.css","assets/datetime.df5e4645.js"]),y=()=>n(()=>import("./jobsView.1b23e6af.js"),["assets/jobsView.1b23e6af.js","assets/jobsView.0cee2fb3.css","assets/iconMaps.853c93a3.js","assets/spyfu-vuex-helpers.esm.87da45d6.js","assets/vendor.6aa46313.js","assets/hostEditor.87575981.js","assets/hostEditor.30ffe716.css","assets/hostSelector.f160872a.js","assets/hostSelector.f5925a7f.css","assets/index.09a07ba8.js","assets/index.a9d6c6c2.js","assets/selectorMixin.fb99fd96.js","assets/numberFormat.773889d3.js","assets/vuedraggable.umd.f0e73259.js"]),oe=()=>n(()=>import("./jobDetailView.59ef861e.js"),["assets/jobDetailView.59ef861e.js","assets/jobDetailView.679af03c.css","assets/datetime.df5e4645.js","assets/iconMaps.853c93a3.js","assets/numberFormat.773889d3.js","assets/timeseries.9158092b.js","assets/vendor.6aa46313.js","assets/fc_textarea.ad33a878.js","assets/fc_textarea.fce54816.css","assets/vue-infinite-loading.33b09e95.js","assets/hostEditor.87575981.js","assets/hostEditor.30ffe716.css","assets/hostSelector.f160872a.js","assets/hostSelector.f5925a7f.css","assets/index.09a07ba8.js","assets/index.a9d6c6c2.js","assets/selectorMixin.fb99fd96.js","assets/hashTable.2fefdc7f.js","assets/jobProgress.9e6a8580.js","assets/jobProgress.8f0ebe4d.css","assets/jobWorkunits.824334d3.js","assets/jobWorkunits.8857a959.css","assets/jobContribution.59d5458b.js","assets/jobContribution.23ad0bc8.css","assets/fc_tile.83a0e997.js"]),le=()=>n(()=>import("./batchList.734ea09d.js"),["assets/batchList.734ea09d.js","assets/vendor.6aa46313.js"]),ce=()=>n(()=>import("./batchDetail.a87308df.js"),["assets/batchDetail.a87308df.js","assets/batchDetail.12346918.css","assets/iconMaps.853c93a3.js","assets/jobWorkunits.824334d3.js","assets/jobWorkunits.8857a959.css","assets/timeseries.9158092b.js","assets/vendor.6aa46313.js","assets/jobContribution.59d5458b.js","assets/jobContribution.23ad0bc8.css","assets/vuedraggable.umd.f0e73259.js"]),de=()=>n(()=>import("./loginView.6aafeb47.js"),["assets/loginView.6aafeb47.js","assets/loginView.ddd96d03.css","assets/logo-light.4fa25edf.js","assets/vendor.6aa46313.js"]),me=()=>n(()=>import("./mainPage.68c53575.js"),["assets/mainPage.68c53575.js","assets/mainPage.5dedcbb4.css","assets/iconMaps.853c93a3.js","assets/spyfu-vuex-helpers.esm.87da45d6.js","assets/vendor.6aa46313.js","assets/vuedraggable.umd.f0e73259.js","assets/logo-light.4fa25edf.js"]),ue=()=>n(()=>import("./addJobView.f90ce632.js"),["assets/addJobView.f90ce632.js","assets/addJobView.c16aa067.css","assets/numberFormat.773889d3.js","assets/iconMaps.853c93a3.js","assets/dictionarySelector.6e2d3817.js","assets/selectorMixin.fb99fd96.js","assets/spyfu-vuex-helpers.esm.87da45d6.js","assets/vendor.6aa46313.js","assets/fileUploader.24c45618.js","assets/fileUploader.db0bfc62.css","assets/fc_textarea.ad33a878.js","assets/fc_textarea.fce54816.css","assets/vue-infinite-loading.33b09e95.js","assets/hostSelector.f160872a.js","assets/hostSelector.f5925a7f.css","assets/index.09a07ba8.js","assets/index.a9d6c6c2.js","assets/datetime.df5e4645.js","assets/hashlistCreator.870b6b41.js","assets/hashlistCreator.c8d9b574.css","assets/hashTypeAutocomplete.772f0815.js"]),he=()=>n(()=>import("./templateList.5dde1078.js"),["assets/templateList.5dde1078.js","assets/templateList.48679128.css","assets/fc_tile.83a0e997.js","assets/vendor.6aa46313.js"]),P=()=>n(()=>import("./hostsView.80ddb0b9.js"),["assets/hostsView.80ddb0b9.js","assets/hostsView.19c0ab7f.css","assets/index.09a07ba8.js","assets/index.a9d6c6c2.js","assets/vendor.6aa46313.js"]),pe=()=>n(()=>import("./hashCacheView.82238f57.js"),["assets/hashCacheView.82238f57.js","assets/hashCacheView.f266a9bb.css","assets/vendor.6aa46313.js"]),_e=()=>n(()=>import("./hashListLister.5d6e02eb.js"),["assets/hashListLister.5d6e02eb.js","assets/hashListLister.4bf70359.css","assets/hashTypeAutocomplete.772f0815.js","assets/vendor.6aa46313.js"]),fe=()=>n(()=>import("./hashListDetail.23aab4ae.js"),["assets/hashListDetail.23aab4ae.js","assets/hashListDetail.8d17a9a8.css","assets/numberFormat.773889d3.js","assets/miniJob.767d3302.js","assets/miniJob.9955af76.css","assets/jobWorkunits.824334d3.js","assets/jobWorkunits.8857a959.css","assets/timeseries.9158092b.js","assets/vendor.6aa46313.js","assets/iconMaps.853c93a3.js","assets/hashTable.2fefdc7f.js"]),S=()=>n(()=>import("./createHashlistView.c2e618c0.js"),["assets/createHashlistView.c2e618c0.js","assets/hashlistCreator.870b6b41.js","assets/hashlistCreator.c8d9b574.css","assets/fileUploader.24c45618.js","assets/fileUploader.db0bfc62.css","assets/fc_textarea.ad33a878.js","assets/fc_textarea.fce54816.css","assets/vue-infinite-loading.33b09e95.js","assets/vendor.6aa46313.js","assets/hashTypeAutocomplete.772f0815.js"]),ve=()=>n(()=>import("./dictionariesView.33a3bb29.js"),["assets/dictionariesView.33a3bb29.js","assets/dictionariesView.15304d18.css","assets/numberFormat.773889d3.js","assets/fc_tile.83a0e997.js","assets/fileUploader.24c45618.js","assets/fileUploader.db0bfc62.css","assets/vendor.6aa46313.js"]),ge=()=>n(()=>import("./dictionaryDetailView.ec08caa0.js"),["assets/dictionaryDetailView.ec08caa0.js","assets/dictionaryDetailView.cd5b9974.css","assets/numberFormat.773889d3.js","assets/fc_textarea.ad33a878.js","assets/fc_textarea.fce54816.css","assets/vue-infinite-loading.33b09e95.js","assets/vendor.6aa46313.js","assets/fc_tile.83a0e997.js"]),be=()=>n(()=>import("./pcfgView.72d9fc10.js"),["assets/pcfgView.72d9fc10.js","assets/pcfgView.17fac215.css","assets/numberFormat.773889d3.js","assets/fc_tile.83a0e997.js","assets/fileCreator.9a368800.js","assets/dictionarySelector.6e2d3817.js","assets/selectorMixin.fb99fd96.js","assets/fileUploader.24c45618.js","assets/fileUploader.db0bfc62.css","assets/vendor.6aa46313.js"]),ye=()=>n(()=>import("./pcfgBrowser.933d6ca4.js"),["assets/pcfgBrowser.933d6ca4.js","assets/pcfgBrowser.24e328de.css","assets/vendor.6aa46313.js"]),ke=()=>n(()=>import("./manageUsersView.59d6c01c.js"),["assets/manageUsersView.59d6c01c.js","assets/manageUsersView.623c2426.css","assets/vendor.6aa46313.js"]),we=()=>n(()=>import("./myAccountView.555d6002.js"),["assets/myAccountView.555d6002.js","assets/myAccountView.7d091aa1.css","assets/vendor.6aa46313.js"]),Ee=()=>n(()=>import("./hostDetailView.3d1da956.js"),["assets/hostDetailView.3d1da956.js","assets/hostDetailView.f4a8c15f.css","assets/numberFormat.773889d3.js","assets/index.09a07ba8.js","assets/index.a9d6c6c2.js","assets/fc_tile.83a0e997.js","assets/resourceMonitor.476c838d.js","assets/timeseries.9158092b.js","assets/vendor.6aa46313.js"]),Ie=()=>n(()=>import("./rulesView.724d3d37.js"),["assets/rulesView.724d3d37.js","assets/rulesView.512ff785.css","assets/numberFormat.773889d3.js","assets/fc_tile.83a0e997.js","assets/fileUploader.24c45618.js","assets/fileUploader.db0bfc62.css","assets/vendor.6aa46313.js"]),De=()=>n(()=>import("./ruleDetailView.d82ffcb6.js"),["assets/ruleDetailView.d82ffcb6.js","assets/ruleDetailView.1e9bc3f3.css","assets/numberFormat.773889d3.js","assets/fc_tile.83a0e997.js","assets/fc_textarea.ad33a878.js","assets/fc_textarea.fce54816.css","assets/vue-infinite-loading.33b09e95.js","assets/vendor.6aa46313.js"]),Ae=()=>n(()=>import("./masksView.1492532e.js"),["assets/masksView.1492532e.js","assets/masksView.b20b6d6a.css","assets/fc_tile.83a0e997.js","assets/fileUploader.24c45618.js","assets/fileUploader.db0bfc62.css","assets/vendor.6aa46313.js"]),Le=()=>n(()=>import("./charsetsView.4ebf47ee.js"),["assets/charsetsView.4ebf47ee.js","assets/charsetsView.01107072.css","assets/numberFormat.773889d3.js","assets/fc_tile.83a0e997.js","assets/fileUploader.24c45618.js","assets/fileUploader.db0bfc62.css","assets/vendor.6aa46313.js"]),je=()=>n(()=>import("./charsetDetailView.6995e6d1.js"),["assets/charsetDetailView.6995e6d1.js","assets/charsetDetailView.e8aa0450.css","assets/numberFormat.773889d3.js","assets/index.09a07ba8.js","assets/index.a9d6c6c2.js","assets/fc_tile.83a0e997.js","assets/vendor.6aa46313.js"]),Oe=()=>n(()=>import("./markovView.0279fc88.js"),["assets/markovView.0279fc88.js","assets/markovView.1bccb375.css","assets/fc_tile.83a0e997.js","assets/fileCreator.9a368800.js","assets/dictionarySelector.6e2d3817.js","assets/numberFormat.773889d3.js","assets/selectorMixin.fb99fd96.js","assets/fileUploader.24c45618.js","assets/fileUploader.db0bfc62.css","assets/vendor.6aa46313.js"]),$e=()=>n(()=>import("./maskDetailView.e71d47d6.js"),["assets/maskDetailView.e71d47d6.js","assets/maskDetailView.9def3be8.css","assets/fc_tile.83a0e997.js","assets/vendor.6aa46313.js"]),Re=()=>n(()=>import("./test.663f374b.js"),["assets/test.663f374b.js","assets/test.1c249e9b.css","assets/vue-infinite-loading.33b09e95.js","assets/vendor.6aa46313.js"]),Te=()=>n(()=>import("./pageNotFound.75ff7414.js"),["assets/pageNotFound.75ff7414.js","assets/pageNotFound.eb13d4c0.css","assets/vendor.6aa46313.js"]),Pe=()=>n(()=>import("./encryptedFilesView.13fd63d6.js"),["assets/encryptedFilesView.13fd63d6.js","assets/encryptedFilesView.61cf1ac3.css","assets/fc_tile.83a0e997.js","assets/vendor.6aa46313.js"]),Se=()=>n(()=>import("./serverMonitor.3c609426.js"),["assets/serverMonitor.3c609426.js","assets/serverMonitor.bc5a24bc.css","assets/resourceMonitor.476c838d.js","assets/timeseries.9158092b.js","assets/vendor.6aa46313.js","assets/datetime.df5e4645.js"]),xe=()=>n(()=>import("./settingsView.19e880b5.js"),["assets/settingsView.19e880b5.js","assets/settingsView.518aa088.css","assets/vendor.6aa46313.js"]),Ce=()=>n(()=>import("./dataTransfer.ebe7bf31.js"),["assets/dataTransfer.ebe7bf31.js","assets/dataTransfer.cd2c0f19.css","assets/vendor.6aa46313.js","assets/iconMaps.853c93a3.js","assets/index.a9d6c6c2.js"]),Ve=()=>n(()=>import("./unauthorized.cd594daf.js"),["assets/unauthorized.cd594daf.js","assets/unauthorized.b19fa34e.css","assets/vendor.6aa46313.js"]);c.use(j);const x=[{path:"/",name:"home",component:ie,meta:{title:"Dashboard",icon:"mdi-view-dashboard"}},{path:"/test",name:"test",component:Re},{path:"/batches",name:"batches",component:le,meta:{title:"Batches",icon:"mdi-tray-full",navtab:0}},{path:"/batches/:id",name:"batch",component:ce,meta:{title:"Batch Detail"}},{path:"/jobs",name:"jobs",component:y,meta:{title:"Jobs",icon:"mdi-briefcase",navtab:0}},{path:"/bins/:id",name:"bins",component:y,meta:{title:"Bins",icon:"mdi-folder",navtab:0}},{path:"/bins/trash",name:"trash",component:y,meta:{title:"Trash",icon:"mdi-delete",navtab:0}},{path:"/jobs/add",name:"addJob",component:ue,meta:{guard:"ADD_NEW_JOB",title:"Create a job",icon:"mdi-briefcase-plus",navtab:0}},{path:"/jobs/:id",name:"jobDetail",component:oe,meta:{title:"Job detail"}},{path:"/templates",name:"templates",component:he,meta:{title:"Job templates",icon:"mdi-seed",navtab:2}},{path:"/hosts",name:"hosts",component:P,meta:{title:"Hosts",icon:"mdi-desktop-classic",navtab:2}},{path:"/hosts/hidden",name:"hiddenHosts",component:P},{path:"/hosts/:id",name:"hostDetail",component:Ee,meta:{title:"Host info"}},{path:"/hashes",name:"hashes",component:pe,meta:{title:"Hashes",icon:"mdi-archive",navtab:1}},{path:"/hashlists",name:"hashlists",component:_e,meta:{title:"Hashlists",icon:"mdi-book-lock",navtab:1}},{path:"/hashlists/create",name:"createHashlist",component:S},{path:"/hashlists/:id",name:"hashlistDetail",component:fe},{path:"/hashlists/:id/extend",name:"hashlistExtend",component:S},{path:"/dictionaries",name:"dictionaries",component:ve,meta:{title:"Dictionaries",icon:"mdi-book-alphabet",navtab:1}},{path:"/dictionaries/:id",name:"dictionaryDetail",component:ge},{path:"/pcfg",name:"pcfg",component:be,meta:{title:"PCFG",icon:"mdi-ray-start-end",navtab:1}},{path:"/pcfg/:id",name:"pcfgBrowser",component:ye,meta:{title:"Inspect PCFG"}},{path:"/rules",name:"rules",component:Ie,meta:{title:"Rules",icon:"mdi-gavel",navtab:1}},{path:"/rules/:id",name:"ruleDetail",component:De},{path:"/charsets",name:"charsets",component:Le,meta:{title:"Charsets",icon:"mdi-alphabetical",navtab:1}},{path:"/charsets/:id",name:"charsetDetail",component:je},{path:"/masks",name:"masks",component:Ae,meta:{title:"Masks",icon:"mdi-guy-fawkes-mask",navtab:1}},{path:"/masks/:id",name:"maskDetail",component:$e},{path:"/markovChains",name:"markovChains",component:Oe,meta:{title:"Markov chains",icon:"mdi-matrix",navtab:1}},{path:"/files",name:"files",component:Pe,meta:{title:"Encrypted Files",icon:"mdi-file-lock",navtab:1}},{path:"/user/manageUsers",name:"manageUsers",component:ke,meta:{guard:"MANAGE_USERS",title:"Manage users",icon:"mdi-folder-account",navtab:2}},{path:"/myAccount",name:"myAccount",component:we,meta:{title:"My account",icon:"mdi-account"}},{path:"/server",name:"server",component:Se,meta:{title:"Server monitor",icon:"mdi-chart-multiline",navtab:2}},{path:"/settings",name:"settings",component:xe,meta:{title:"Settings",icon:"mdi-tune",navtab:2}},{path:"/transfer",name:"transfer",component:Ce,meta:{title:"Data Transfer",icon:"mdi-dolly",navtab:2}},{path:"/error/unauthorized",name:"unauthorized",component:Ve,meta:{title:"Access Denied",icon:"mdi-alert"}},{path:"*",component:Te}],h=new j({mode:"history",routes:[{path:"/login",name:"login",component:de},{path:"",component:me,children:x}]});h.beforeResolve(async(e,s,t)=>{const r=e.meta.guard;if(!r){t();return}await w.dispatch("hasPermission",r)?t():t({name:"unauthorized",query:{accessing:e.path,missing:r}})});h.afterEach((e,s)=>{const t=h.app.$store.state.project||"Fitcrack";e.meta.title?document.title=`${e.meta.title} \u2013 ${t}`:document.title=t});function mt(e){const s=x.find(t=>t.name===e);return s.meta&&s.meta.icon?s.meta.icon:null}const k=window.serverAddress,C={id:null,username:null,mail:null,role:{MANAGE_USERS:!1,ADD_NEW_JOB:!1,UPLOAD_DICTIONARIES:!1,VIEW_ALL_JOBS:!1,EDIT_ALL_JOBS:!1,OPERATE_ALL_JOBS:!1,ADD_USER_PERMISSIONS_TO_JOB:!1}};var Ne={state:{loggedIn:!1,userData:C},getters:{can:e=>s=>e.userData.role[s]},mutations:{_setUser(e,s){s?(e.loggedIn=!0,e.userData=s):(e.loggedIn=!1,e.userData=C)}},actions:{async refreshUser({commit:e}){const{user:s,loggedIn:t,token:r}=await d.get(`${k}/user/isLoggedIn`).then(a=>a.data);return localStorage.setItem("jwt",r),e("_setUser",t?s:null),s},async resume({state:e,dispatch:s}){return e.loggedIn?!0:(await s("refreshUser")).id!=null},async signIn({commit:e},s){try{const{user:t,token:r}=await d.post(`${k}/user/login`,s).then(i=>i.data);localStorage.setItem("jwt",r),e("_setUser",t);const a=sessionStorage.getItem("loginRedirect");return sessionStorage.removeItem("loginRedirect"),!a||a==="/login"?h.replace({name:"home"}):h.push(a),!0}catch(t){return console.error(t),e("_setUser",null),!1}},async signOut({commit:e}){await d.get(`${k}/user/logout`),e("_setUser",null),h.push({name:"login"})},async hasPermission({state:e,dispatch:s},t){return e.loggedIn||await s("resume"),e.userData.role[t]}}};const V={selectedTemplate:0,step:1,name:"",hashListId:null,hosts:[],startDate:null,endDate:null},v={template:"Empty",attackSettingsTab:void 0,comment:"",leftDicts:[],rightDicts:[],rules:[],ruleLeft:"",ruleRight:"",masks:[""],hybridMask:"",charset:[],submode:0,distributionMode:0,markov:[],markovThresh:NaN,pcfg:[],keyspaceLimit:0,checkDuplicates:!0,casePermute:!1,minPasswordLen:1,maxPasswordLen:8,minElemInChain:1,maxElemInChain:8,generateRandomRules:0,optimized:!0,startNow:!0,endNever:!0,timeForJob:void 0};var Me={namespaced:!0,state:u(u({},V),v),getters:{attackSettings(e){const s=Ue.find(t=>t.handler===e.attackSettingsTab);return s?{attack_mode:s.id,attack_name:s.serverName,rules:e.rules.length>0?e.rules[0]:null,left_dictionaries:e.leftDicts,right_dictionaries:e.rightDicts,rule_left:e.ruleLeft,rule_right:e.ruleRight,masks:e.masks,attack_submode:e.submode,distribution_mode:parseInt(e.distributionMode),markov_treshold:e.submode==0?null:parseInt(e.markovThresh),markov:e.submode==0?null:e.markov[0],charset:e.charset,mask:e.hybridMask,pcfg_grammar:e.pcfg[0],keyspace_limit:e.keyspaceLimit,check_duplicates:e.checkDuplicates,case_permute:e.casePermute,min_password_len:parseInt(e.minPasswordLen),max_password_len:parseInt(e.maxPasswordLen),min_elem_in_chain:parseInt(e.minElemInChain),max_elem_in_chain:parseInt(e.maxElemInChain),generate_random_rules:parseInt(e.generateRandomRules),optimized:c.prototype.$optimizedOnly?!0:e.optimized}:!1},jobSettings(e,{attackSettings:s}){return{name:e.name,comment:e.comment,hosts_ids:e.hosts.map(t=>t.id),seconds_per_job:parseInt(e.timeForJob),time_start:e.startNow?"":e.startDate,time_end:e.endNever?"":e.endDate,attack_settings:s,hash_list_id:e.hashListId}},validAttackSpecificSettings(e){switch(e.attackSettingsTab){case"dictionary":return e.leftDicts.length>0;case"combinator":return e.leftDicts.length>0&&e.rightDicts.length>0;case"maskattack":return e.submode>0&&e.markov.length==0?!1:e.masks.filter(s=>s!=="").length>0;case"hybridWordlistMask":return e.leftDicts.length>0&&e.hybridMask!=="";case"hybridMaskWordlist":return e.rightDicts.length>0&&e.hybridMask!=="";case"pcfgAttack":return e.pcfg.length>0;case"princeAttack":return!(e.leftDicts.length==0||e.minPasswordLen<=0||e.maxPasswordLen<=0||e.maxPasswordLen>32||e.maxElemInChain>16||e.minElemInChain<=0||e.maxElemInChain<=0||e.minPasswordLen>e.maxPasswordLen||e.minElemInChain>e.maxElemInChain||e.maxElemInChain>e.maxPasswordLen||e.keyspaceLimit<0||e.generateRandomRules<0||e.rules.length>0&&e.generateRandomRules>0);default:return!0}},valid(e,{attackSettings:s,validAttackSpecificSettings:t}){return!e.attackSettingsTab||!s||e.hashListId==null||e.timeForJob<10||e.name===""?!1:t},keyspaceKnown({attackSettingsTab:e},{validAttackSpecificSettings:s}){return e&&s},template(e){const s=Object.keys(v);return Object.keys(e).filter(t=>s.includes(t)).reduce((t,r)=>((JSON.stringify(e[r])!==JSON.stringify(v[r])||r==="masks"&&e.masks[0]!=="")&&(t[r]=e[r]),t),{})},validTemplate(e,{template:s}){return Object.keys(s).length>0},makeTemplate(e,{template:s}){return function(t){const r=s;return r.template=t,r}}},mutations:b(u({},M(u(u({},V),v))),{update(e,s){Object.assign(e,s)},applyTemplate(e,s="{}"){Object.assign(e,u(u({},v),s))},addMask({masks:e},s=""){e.push(s)},deleteMask({masks:e},s){e.length<=1||e.splice(s,1)},updateMask({masks:e},{index:s,val:t}){c.set(e,s,t)},mergeMasks(e,s){e.masks=[...e.masks,...s],e.masks[0]==""&&e.masks.shift()}})};const Ue=[{handler:"dictionary",name:"Dictionary",id:0,serverName:"dict"},{handler:"combinator",name:"Combination",id:1,serverName:"combinator"},{handler:"maskattack",name:"Brute-force",id:3,serverName:"mask"},{handler:"hybridWordlistMask",name:"Hybrid Wordlist + Mask",id:6,serverName:"Hybrid wordlist+mask"},{handler:"hybridMaskWordlist",name:"Hybrid Mask + Wordlist",id:7,serverName:"Hybrid mask+wordlist"},{handler:"princeAttack",name:"PRINCE",id:8,serverName:"prince"},{handler:"pcfgAttack",name:"PCFG",id:9,serverName:"pcfg"}],_=window.serverAddress,N={loading:!1,clean:!0,bins:[],selectedJobs:[]};var Je={namespaced:!0,state:N,getters:{},mutations:b(u({},M(N)),{loading(e){e.loading=!0},idle(e){e.loading=!1},dirty(e){e.clean=!1},clean(e){e.clean=!0},unselect(e){e.selectedJobs=[]},populate(e,s){e.bins=s},rename(e,{id:s,name:t}){e.bins.find(r=>r.id==s).name=t}}),actions:{async load({commit:e}){e("loading");const s=await d.get(`${_}/bins`).then(t=>t.data.items);e("idle"),e("populate",s)},async create({commit:e,dispatch:s},t){e("loading"),await d.post(`${_}/bins`,{name:t}),s("load")},move({commit:e,state:s},{id:t,position:r,oldIndex:a}){return e("loading"),d.patch(`${_}/bins/${t}/move`,{position:r}).catch(i=>{console.warn("Undoing move operation on bins");const o=s.bins,f=o[r];o.splice(r,1),o.splice(a,0,f),e("populate",o)}).finally(()=>{e("idle")})},async assign({commit:e,dispatch:s},{id:t,payload:r,dirty:a}){e("loading"),await d.put(`${_}/bins/${t}/assign`,r),e("unselect"),a&&e("dirty"),s("load")},async rename({commit:e},{id:s,newName:t}){e("loading");try{await d.patch(`${_}/bins/${s}`,{name:t}),e("rename",{id:s,name:t})}catch{console.error("Rename failed")}finally{e("idle")}},async delete({commit:e,dispatch:s,state:{bins:t}},r){e("loading"),await d.delete(`${_}/bins/${r}`);const a=t.map(i=>i.id).indexOf(r);a>0?h.replace({name:"bins",params:{id:t[a-1].id}}):h.replace({name:"jobs"}),s("load")}}},Fe={namespaced:!0,state:{jobs:[]},getters:{job_ids:e=>e.jobs.map(s=>s.id)},mutations:{add(e,s){const t=s.filter(r=>!e.jobs.some(a=>r.id==a.id));e.jobs=e.jobs.concat(t)},remove(e,s){e.jobs=e.jobs.filter(t=>t.id!=s)},reset(e){e.jobs=[]}}};c.use(O);var w=new O.Store({state:{darkAppearance:!1,project:window.projectName||"Fitcrack"},modules:{user:Ne,jobForm:Me,binInterface:Je,transfer:Fe}});function ut(e){const s={};return e.forEach(t=>{s[t]=`${t}Mut`}),s}function M(e){const s=r=>(a,i)=>{a[r]=i},t={};return Object.keys(e).forEach(r=>{t[`${r}Mut`]=s(r)}),t}var He=function(){var e=this,s=e.$createElement,t=e._self._c||s;return t("v-dialog",{attrs:{"max-width":e.options.width},model:{value:e.dialog,callback:function(r){e.dialog=r},expression:"dialog"}},[t("v-card",{attrs:{flat:""}},[t("v-card-title",[e._v(" "+e._s(e.title)+" ")]),t("v-card-text",{directives:[{name:"show",rawName:"v-show",value:!!e.message,expression:"!!message"}]},[e._v(" "+e._s(e.message)+" ")]),t("v-card-actions",[t("v-spacer"),t("v-btn",{attrs:{text:""},nativeOn:{click:function(r){return e.cancel()}}},[e._v(" Cancel ")]),t("v-btn",{attrs:{color:"primary",outlined:"",text:""},nativeOn:{click:function(r){return e.agree()}}},[t("v-icon",{attrs:{left:""}},[e._v(" mdi-keyboard-return ")]),e._v(" Yes ")],1)],1)],1)],1)},Be=[];function E(e,s,t,r,a,i,o,f){var l=typeof e=="function"?e.options:e;s&&(l.render=s,l.staticRenderFns=t,l._compiled=!0),r&&(l.functional=!0),i&&(l._scopeId="data-v-"+i);var p;if(o?(p=function(m){m=m||this.$vnode&&this.$vnode.ssrContext||this.parent&&this.parent.$vnode&&this.parent.$vnode.ssrContext,!m&&typeof __VUE_SSR_CONTEXT__!="undefined"&&(m=__VUE_SSR_CONTEXT__),a&&a.call(this,m),m&&m._registeredComponents&&m._registeredComponents.add(o)},l._ssrRegister=p):a&&(p=f?function(){a.call(this,(l.functional?this.parent:this).$root.$options.shadowRoot)}:a),p)if(l.functional){l._injectStyles=p;var H=l.render;l.render=function(B,D){return p.call(D),H(B,D)}}else{var I=l.beforeCreate;l.beforeCreate=I?[].concat(I,p):[p]}return{exports:e,options:l}}const We={data(){return{dialog:!1,resolve:null,reject:null,message:null,title:null,options:{color:"primary",width:290}}},mounted(){window.addEventListener("keydown",this.shortcutHandler)},beforeDestroy(){window.removeEventListener("keydown",this.shortcutHandler)},methods:{open(e,s,t){return this.dialog=!0,this.title=e,this.message=s,this.options=Object.assign(this.options,t),new Promise((r,a)=>{this.resolve=r,this.reject=a})},agree(){this.resolve(!0),this.reject(!1),this.dialog=!1},cancel(){this.dialog=!1},shortcutHandler(e){switch(e.key){case"Escape":this.reject&&this.cancel();break;case"Enter":this.resolve&&this.agree();break;default:return}}}},U={};var ze=E(We,He,Be,!1,Ge,null,null,null);function Ge(e){for(let s in U)this[s]=U[s]}var Ke=function(){return ze.exports}(),Ye=function(){var e=this,s=e.$createElement,t=e._self._c||s;return t("v-dialog",{attrs:{"max-width":"550",persistent:""},model:{value:e.visible,callback:function(r){e.visible=r},expression:"visible"}},[t("v-card",[t("v-card-title",[e._v(" API Server "+e._s(e.retries>5?"Really ":"")+"Unreachable ")]),t("v-card-subtitle",[e._v(" "+e._s(e.addr)+" ")]),t("v-card-text",[e.retries<=5?t("span",[e._v(" We'll keep trying to connect in case it hasn't started yet. ")]):t("span",[e._v(" Tried "+e._s(e.retries)+" times and still nothing. "),e.retries>=15?t("span",[e._v(" Sorry, but at this point, it's pretty clear you'll need to fix it manually.")]):e._e(),e.retries>=20?t("span",[e._v(" Seriously, it won't just magically start working.")]):e._e(),e.retries>=30?t("span",[e._v(" We're getting nowhere like this. Just follow the "),t("strong",[e._v("Help button")]),e._v(" below, please?")]):e._e(),e.retries>=50?t("span",[t("br"),t("br"),e._v("You're enjoying this, aren't you?")]):e._e(),e.retries>=60?t("span",[e._v(" It's kind of awkward, not gonna lie.")]):e._e(),e.retries>=70?t("span",[e._v(" You do realize that this is just an error message, right?")]):e._e(),e.retries>=100?t("span",[t("br"),t("br"),e._v("OK, it's your free time, not ours. Let's humor you some more, then.")]):e._e()]),t("v-progress-linear",{staticClass:"mt-3",attrs:{indeterminate:"",color:e.statusColor}})],1),t("v-card-text",{staticClass:"py-0"},[e.retries<=0?t("v-alert",{attrs:{type:"info"}},[e._v(" Hang tight! Reconnecting in a few. ")]):e.retries<=5?t("v-alert",{attrs:{type:"error",color:e.statusColor}},[e._v(" "+e._s(e.retries)+" failed reconnection attempt"+e._s(e.retries>1?"s":"")+" ")]):e.retries<100?t("v-alert",{attrs:{type:"error",icon:"mdi-emoticon-dead"}},[e._v(" Yeah, still no connection. Try "),t("strong",[e._v("Help")]),e._v(" below. ")]):t("v-alert",{attrs:{type:"success",icon:"mdi-trophy"}},[e._v(" Yay, "),t("strong",[e._v("Achievement Unlocked!")]),e._v(" Retried "+e._s(Math.floor(e.retries/10)*10)+" times. ")])],1),t("v-card-actions",[t("v-spacer"),t("v-btn",{attrs:{text:"",color:e.retries>=3?e.statusColor:""},on:{click:e.showSuggestions}},[e._v(" Help ")]),t("v-btn",{on:{click:e.retry}},[e._v(" Retry Now ")])],1)],1),t("v-bottom-sheet",{attrs:{"overlay-color":"secondary","overlay-opacity":"0.3",dark:""},model:{value:e.help,callback:function(r){e.help=r},expression:"help"}},[t("v-card",{attrs:{tile:"","min-width":"100vw"}},[t("v-card-title",[t("v-icon",{attrs:{left:""}},[e._v("mdi-bandage")]),e._v(" Here's what you can try "),t("v-spacer"),t("v-card-actions",[t("v-btn",{attrs:{text:"",to:"//github.com/nesfit/fitcrack/issues",target:"_blank"}},[t("v-icon",{attrs:{left:""}},[e._v("mdi-github")]),e._v(" Ask on Github ")],1),t("v-btn",{attrs:{color:"success"},on:{click:e.hideSuggestions}},[t("v-icon",{attrs:{left:""}},[e._v("mdi-check")]),e._v(" Done ")],1)],1)],1),t("v-card-text",[t("v-expansion-panels",{attrs:{multiple:"",light:""},model:{value:e.suggestionsShown,callback:function(r){e.suggestionsShown=r},expression:"suggestionsShown"}},[t("v-expansion-panel",[t("v-expansion-panel-header",[t("span",[t("v-icon",{staticClass:"pb-1",attrs:{left:"",small:""}},[e._v("mdi-ethernet")]),t("span",{staticClass:"text-subtitle-1"},[e._v("Check network connection")])],1)]),t("v-expansion-panel-content",{staticClass:"limit-width"},[e._v(" Are you connected to the right network? Your Fitcrack API server might be exposed to a different network, or your internet connection maybe isn't working right and the app loaded out of cache. "),t("ul",[t("li",[e._v("Check your internet connection")]),t("li",[e._v("Check where you are \u2013 do you need a VPN to connect to Fitcrack?")]),t("li",[e._v("If you are on the same network as the server, try to ping it")])])])],1),t("v-expansion-panel",[t("v-expansion-panel-header",[t("span",[t("v-icon",{staticClass:"pb-1",attrs:{left:"",small:""}},[e._v("mdi-wrench")]),t("span",{staticClass:"text-subtitle-1"},[e._v("Check client configuration")])],1)]),t("v-expansion-panel-content",{staticClass:"limit-width"},[e._v(" The webadmin app is configured to connect to the API on a specific address. Check if it hasn't changed server-side, or if it was configured correctly in the first place. "),t("ul",[t("li",[e._v("Check if "),t("code",[e._v("window.serverAddress")]),e._v(" in the "),t("code",[e._v("/public/static/configuration.js")]),e._v(" file of FitcrackFE has the correct address")]),t("li",[e._v("Double-check the server's current address!")])])])],1),t("v-expansion-panel",[t("v-expansion-panel-header",[t("span",[t("v-icon",{staticClass:"pb-1",attrs:{left:"",small:""}},[e._v("mdi-server")]),t("span",{staticClass:"text-subtitle-1"},[e._v("Check server status")])],1)]),t("v-expansion-panel-content",{staticClass:"limit-width"},[e._v(" The API server could be offline or malfunctioning. You could try to restart the Fitcrack service. "),t("ul",[t("li",[e._v("Access the server shell and check if the service is enabled with "),t("code",[e._v("service fitcrack status")])]),t("li",[e._v("Try to start it up if needed with "),t("code",[e._v("service fitcrack start")])])])])],1)],1)],1)],1)],1)],1)},qe=[];const Xe={props:{visible:Boolean},data(){return{interval:null,retries:0,help:!1,suggestionsShown:[0]}},computed:{addr(){return window.serverAddress},statusColor(){return this.retries==0?"blue":this.retries<=5?"orange":this.retries<100?"red":"green"}},methods:{showSuggestions(){this.help=!0},hideSuggestions(){this.help=!1},async retry(){this.retries+=1,await w.dispatch("refreshUser")&&this.$emit("connect")}},watch:{visible(e){e==!0?(this.retries=0,setTimeout(this.retry,2e3),this.interval=setInterval(this.retry,5e3)):clearInterval(this.interval)}}},J={};var Qe=E(Xe,Ye,qe,!1,Ze,"43e0bf24",null,null);function Ze(e){for(let s in J)this[s]=J[s]}var et=function(){return Qe.exports}(),tt=function(){var e=this,s=e.$createElement,t=e._self._c||s;return t("v-app",[t("Connection",{attrs:{visible:e.connectionAssistant},on:{connect:e.handleReconnection}}),t("v-snackbar",{staticClass:"errorSnackbar pa-0",attrs:{timeout:6e3,bottom:"",right:"",color:"transparent"},model:{value:e.alert,callback:function(r){e.alert=r},expression:"alert"}},[t("v-alert",{staticClass:"height100 ma-0 width100",attrs:{type:e.alertType,value:!0,dismissible:"",transition:"none"},model:{value:e.alert,callback:function(r){e.alert=r},expression:"alert"}},[e._v(" "+e._s(e.alertText)+" ")])],1),t("router-view",{attrs:{"is-dark":e.isDark},on:{alert:e.setAlert}}),t("confirm",{ref:"confirm"})],1)},st=[];const rt={name:"App",components:{confirm:Ke,Connection:et},data:function(){return{alert:!1,alertText:"",alertType:"error",isDark:!1,connectionAssistant:!1}},created:function(){this.axios.interceptors.request.use(function(t){const r=localStorage.getItem("jwt");return r&&r!==""&&(t.headers.common.Authorization=`Bearer ${r}`),t}.bind(this),function(t){return Promise.reject(t)}.bind(this)),this.axios.interceptors.response.use(function(t){return t.data.hasOwnProperty("hideAlert")&&t.data.hideAlert&&(this.alert=!1),t.data.hasOwnProperty("status")&&t.data.hasOwnProperty("message")&&t.data.status&&(this.alertType="success",this.alert=!0,this.alertText=t.data.message),t}.bind(this),function(t){if(this.alertType="error",this.alert=!0,t.response!==void 0)if(t.response.data.message==="Input payload validation failed"){let r=Object.keys(t.response.data.errors)[0];this.alertText="Wrong settings. "+r+" ("+t.response.data.errors[r]+")"}else this.alertText=t.response.data.message;else t.message!==void 0?t.message==="Network Error"?(this.alert=!1,this.connectionAssistant=!0):this.alertText=t.message:this.alertText="Can not connect to "+this.$serverAddr;return Promise.reject(t)}.bind(this));const e=window.matchMedia("(prefers-color-scheme: dark)");e.addListener(t=>{const r=localStorage.getItem("appearance");(!r||r=="auto")&&(this.$vuetify.theme.dark=t.matches)});const s=localStorage.getItem("appearance");s&&s=="dark"?this.$vuetify.theme.dark=!0:(!s||s=="auto")&&(this.$vuetify.theme.dark=e.matches),window.addEventListener("keypress",t=>{localStorage.getItem("testmode")=="true"&&t.ctrlKey&&t.shiftKey&&t.code=="KeyL"&&(this.$vuetify.theme.dark=!this.$vuetify.theme.dark)})},mounted(){this.$root.$confirm=this.$refs.confirm.open},methods:{setAlert({type:e,text:s}){this.alertType=e,this.alertText=s,this.alert=!0},handleReconnection(){this.connectionAssistant=!1,this.$router.go()}}},F={};var at=E(rt,tt,st,!1,nt,"64a0d094",null,null);function nt(e){for(let s in F)this[s]=F[s]}var it=function(){return at.exports}();const ot={install(e){e.prototype.$logoutUser=function(){this.$store.state.user={userData:{id:null,username:null,mail:null,role:{MANAGE_USERS:!1,ADD_NEW_JOB:!1,UPLOAD_DICTIONARIES:!1,VIEW_ALL_JOBS:!1,EDIT_ALL_JOBS:!1,OPERATE_ALL_JOBS:!1,ADD_USER_PERMISSIONS_TO_JOB:!1}},loggedIn:!1},console.log("logged OUT!!!"),this.$router.push({name:"login"})},e.prototype.$logInUser=function(s){this.$store.state.user.userData=s,this.$store.state.user.loggedIn=!0,console.log("logged IN!!!");const t=sessionStorage.getItem("loginRedirect");sessionStorage.removeItem("loginRedirect"),!t||t==="/login"?this.$router.replace({name:"home"}):this.$router.replace(t)},e.prototype.$currentUser=function(){return this.axios.get(this.$serverAddr+"/user/isLoggedIn").then(s=>s.data)},e.prototype.$needLogin=!0,e.prototype.$serverAddr=window.serverAddress,e.prototype.$docsLink=window.docsLink,e.prototype.$optimizedOnly=window.optimizedKernelsOnly,e.prototype.$user=function(){return this.user},e.prototype.$username=this.name,e.prototype.$userCan=function(s){return this.$store.state.user.userData.role[s]},e.prototype.$error=function(s){this.axios.interceptors.response.handlers["0"].rejected({message:s})},e.prototype.$success=function(s){this.axios.interceptors.response.handlers["0"].fulfilled({data:{status:!0,message:s}})},e.prototype.$hideAlert=function(){this.axios.interceptors.response.handlers["0"].fulfilled({data:{hideAlert:!0}})}}},g="fitcrack-pagination-store";var lt={install(e){localStorage.getItem(g)||localStorage.setItem(g,"{}"),e.prototype.$paginator=function(s,t){const r=JSON.parse(localStorage.getItem(g));return t===void 0?r[s]||1:(t===null?delete r[s]:r[s]=t,localStorage.setItem(g,JSON.stringify(r)),r[s])}}};q.register(...X);document.title=window.projectName||"Fitcrack";Q($);c.use(Z);c.use(ee);c.use(te,{moment:$});c.use(se,d);c.axios.defaults.withCredentials=!0;c.use(R);c.use(ot);c.use(lt);new c({el:"#app",router:h,store:w,vuetify:new R({icons:{iconfont:"mdi"},theme:{dark:!1,themes:window.theme}}),render:e=>e(it)}).$mount("#app");export{Ue as a,E as n,mt as r,w as s,ut as t};
