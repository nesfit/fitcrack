import{n as r}from"./index.2fcccaca.js";var u=function(){var t=this,e=t.$createElement,a=t._self._c||e;return a("v-autocomplete",t._b({attrs:{value:t.value,autofocus:t.autofocus,clearable:"",items:t.hashTypes,"item-text":"name","return-object":"","hide-details":"","no-data-text":"No matching hash type"},on:{input:function(s){return t.$emit("input",s)}},scopedSlots:t._u([{key:"item",fn:function(s){var o=s.item;return[a("v-list-item-content",[a("v-list-item-title",[a("b",[t._v(t._s(o.code))]),t._v(" - "+t._s(o.name))])],1)]}}])},"v-autocomplete",t.$attrs,!1))},c=[];const i={name:"HashTypeAutocomplete",props:{autofocus:{type:Boolean,default:!1},value:{type:Object,default:()=>{}}},data(){return{hashTypes:[]}},mounted(){this.loadHashTypes()},methods:{loadHashTypes(){this.axios.get(this.$serverAddr+"/hashcat/hashTypes").then(t=>{this.hashTypes=t.data.hashtypes})}}},n={};var l=r(i,u,c,!1,h,null,null,null);function h(t){for(let e in n)this[e]=n[e]}var m=function(){return l.exports}();export{m as H};