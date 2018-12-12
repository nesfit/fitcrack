/*
 * Author : see AUTHORS
 * Licence: MIT, see LICENSE
 */

#ifndef NULLPTR_HPP
#define NULLPTR_HPP
/// When using C++98 declare nullptr from C++11 so we can use it instead of NULL which has some function overriding issues
#if __cplusplus == 199711L
/** 
 * Self defined nullptr same or similar way as in C++11, motivation was to use
 * C++ as much as possible and NULL doesn't have to be type save it is just 0
 * */
const                         /* this is a const object...     */
class nullptr_t {
    public:
	/**
	 * @brief   Null non-member pointer of null convertible type
	 * @return  Null 
	 */
	template<class T>      
	    operator T*() const { 
		return 0; 
	    }         

	/**
	 * @brief    Null non-member pointer of any type
	 * @tparam C Class
	 * @tparam T Type
	 * @return   Null
	 */
	template<class C, class T>
	    operator T C::*() const{ 
		return 0; 
	    }   

    private:
       /**
	* @brief   Can't take adress of nullptr
	*/
       void operator&() const;  
} nullptr = {};     

#endif // _cplusplus
#endif // NULLPTR_HPP
