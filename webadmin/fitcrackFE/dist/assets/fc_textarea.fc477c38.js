import{I as i}from"./vue-infinite-loading.33b09e95.js";import{n}from"./index.0b60da2f.js";var l=function(){var t=this,s=t.$createElement,e=t._self._c||s;return e("div",{staticClass:"textAreaCont"},[t.searchEnable?e("v-row",{staticClass:"mx-2 mb-2"},[e("v-text-field",{staticClass:"ml-3",attrs:{"single-line":"","hide-details":"",label:"Search",clearable:""},on:{"click:clear":t.clearSearch,keypress:function(a){return!a.type.indexOf("key")&&t._k(a.keyCode,"enter",13,a.key,"Enter")?null:t.search.apply(null,arguments)}},model:{value:t.searchText,callback:function(a){t.searchText=a},expression:"searchText"}}),e("v-btn",{staticClass:"mt-2",attrs:{icon:"",color:"primary"},on:{click:t.search}},[e("v-icon",[t._v("search")])],1)],1):t._e(),e("div",{staticClass:"scrollCont",class:{editable:!t.readonly},style:t.style},[t.noResults?e("div",{staticClass:"text-center pt-4"},[t._v(" No result found. ")]):t.loading?e("div",{staticClass:"text-center pa-5"},[e("v-progress-circular",{attrs:{slot:"spinner",size:"50",width:3,indeterminate:"",color:"primary"},slot:"spinner"})],1):e("v-row",{staticClass:"minheight200 mx-2"},[e("div",{staticClass:"lineNumberCont pr-2 pl-3 pt-2"},t._l(t.linesCount,function(a){return e("p",{key:a,staticClass:"pa-0 ma-0"},[t._v(" "+t._s(a)+" ")])}),0),e("div",{staticClass:"textarea-wrap"},[t.searching?e("textarea",{directives:[{name:"model",rawName:"v-model",value:t.searchData,expression:"searchData"}],staticClass:"pt-2 pl-2",attrs:{readonly:!0,wrap:"off"},domProps:{value:t.searchData},on:{input:function(a){a.target.composing||(t.searchData=a.target.value)}}}):e("textarea",{directives:[{name:"model",rawName:"v-model",value:t.textareaData,expression:"textareaData"}],ref:"textarea",staticClass:"pt-2 pl-2",attrs:{readonly:t.readonly,wrap:"off"},domProps:{value:t.textareaData},on:{input:[function(a){a.target.composing||(t.textareaData=a.target.value)},t.update],blur:t.emmitBlur,focus:t.emmitFocus}}),t._v(" "),t.url&&!t.searching?e("infinite-loading",{on:{infinite:t.loadData}},[e("v-progress-circular",{attrs:{slot:"spinner",size:"50",width:3,indeterminate:"",color:"primary"},slot:"spinner"}),e("span",{attrs:{slot:"no-more"},slot:"no-more"},[t._v(" You reached the end of file. ")]),e("span",{attrs:{slot:"no-results"},slot:"no-results"},[t._v(" You reached the end of file. ")])],1):t._e()],1),t._t("after"),t.canRemoveLine?e("div",{staticClass:"pt-2"},t._l(t.linesCount,function(a){return e("p",{key:a,staticClass:"pa-0 ma-0"},[e("v-icon",{staticClass:"removeLineBtn",attrs:{small:"",color:"error"},on:{click:function(p){return t.removeLine(a)}}},[t._v(" close ")])],1)}),0):t._e()],2)],1)],1)},o=[];const c={name:"FcTextarea",components:{InfiniteLoading:i},props:{maxHeight:String,readonly:Boolean,url:String,searchEnable:Boolean,canRemoveLine:Boolean,value:{type:String,default:""}},data(){return{data:"",page:1,searching:!1,searchText:"",searchData:"",noResults:!1,loading:!1,textareaData:""}},computed:{linesCount:function(){return this.searching?this.searchData.split(`
`).length-1:this.value?this.value.split(`
`).length:1},style(){return this.maxHeight?"max-height: "+this.maxHeight+"px":""}},watch:{value:function(){this.textareaData=this.value}},mounted:function(){this.textareaData=this.value},methods:{focus(){this.$refs.textarea.focus()},emmitBlur(){this.$emit("blur",this.value)},emmitFocus(){this.$emit("focus",this.value)},update(){this.readonly||this.$emit("input",this.textareaData)},loadData:function(t){this.axios.get(this.url,{params:{page:this.page,per_page:100}}).then(s=>{s.data.status?(this.value+=s.data.data,this.page++,t.loaded()):t.complete()})},search:function(){this.noResults=!1,this.loading=!0,this.searching=!0,this.searchData="",this.axios.get(this.url,{params:{search:this.searchText}}).then(t=>{t.data.status?this.searchData=t.data.data:(this.noResults=!0,this.searchData=""),this.loading=!1})},clearSearch:function(){this.searching=!1,this.noResults=!1},removeLine:function(t){var s=this.textareaData.split(`
`);s.splice(t-1,1),this.textareaData=s.join(`
`),this.$emit("input",this.textareaData),this.$emit("blur",this.textareaData)}}},r={};var u=n(c,l,o,!1,h,"7c0f6452",null,null);function h(t){for(let s in r)this[s]=r[s]}var f=function(){return u.exports}();export{f};
