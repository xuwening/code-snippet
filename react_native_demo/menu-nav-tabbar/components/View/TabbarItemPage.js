
import React, {Component} from 'react';
import {

	View,
	TouchableOpacity,
	StyleSheet,
	Text,
	Image,
	WebView,
} from 'react-native';

const SecondView = require('./SecondView');
const Swiper = require('react-native-swiper');
var LDWebViewPage = require('./LDWebViewPage');

const ImageRes = {

	pic1: 'http://clientdispatch.10086.cn:8080/group1/M00/00/89/rBCJYFc4z5KAQNoVAADNJiv-Evs291.jpg',
	pic2: 'http://clientdispatch.10086.cn:8080/group2/M00/00/B6/rBCJYlc1h1qAJ49fAAD9sV_PWEI736.jpg',
	pic3: 'http://clientdispatch.10086.cn:8080/group1/M00/00/79/rBCJYFcV-veASvCrAAEpvXolWQ0014.jpg',
	pic4: 'http://clientdispatch.10086.cn:8080/group1/M00/00/89/rBCJYFc4z5KAQNoVAADNJiv-Evs291.jpg',
	pic5: 'http://clientdispatch.10086.cn:8080/group2/M00/00/89/rBCJYlcLQIKAFRiTAAEQvO57vIY526.jpg',
	pic6: 'http://clientdispatch.10086.cn:8080/group2/M00/00/BE/rBCJYlc57yOAXO1wAADfam9Cm7c255.jpg',
	pic7: 'http://clientdispatch.10086.cn:8080/group2/M00/00/6A/rBCJYlcClQGAXUEKAAAFA0UuBcM986.png',
	pic8: 'http://clientdispatch.10086.cn:8080/group2/M00/00/6A/rBCJYlcClVKAanxGAAAEeaaBXp4227.png',
	pic9: 'http://clientdispatch.10086.cn:8080/group2/M00/00/6A/rBCJYlcClbiAf8AGAAAK-YtwcDI034.png',

	icon1: {
		title: '我的账单',
		iconUrl: 'http://clientdispatch.10086.cn:8080/group2/M00/00/71/rBCJYlcDoP6AKnL6AAAEm2iPrRg271.png',

		type: 1,
		actionUrl: '',
	},

	icon2: {
		title: '套餐余量',
		iconUrl: 'http://clientdispatch.10086.cn:8080/group1/M00/00/66/rBCJYFcDoDeAEF81AAAF6tjhGCE109.png',

		type: 1,
		actionUrl: '',
	},

	icon3: {
		title: '已订购业务',
		iconUrl: 'http://clientdispatch.10086.cn:8080/group1/M00/00/62/rBCJYVcCYJyAVUIbAAAEJQCNidY160.png',

		type: 1,
		actionUrl: '',
	},

	icon4: {
		title: '话费余额',
		iconUrl: 'http://clientdispatch.10086.cn:8080/group1/M00/00/62/rBCJYFcCemuAQ5YMAAAFeVkkwlU314.png',

		type: 1,
		actionUrl: '',
	},

	icon5: {
		title: '详单查询',
		iconUrl: 'http://clientdispatch.10086.cn:8080/group2/M00/00/6A/rBCJYlcCkNWAFCIlAAADvWo0AJA221.png',

		type: 1,
		actionUrl: '',
	},

	icon6: {
		title: '充值缴费',
		iconUrl: 'http://clientdispatch.10086.cn:8080/group2/M00/00/71/rBCJYlcDn92AeD9xAAAIAhYn2gU341.png',

		type: 2,
		actionUrl: 'https://clientdispatch.10086.cn/leadeon-cmcc-static/v2.0/pages/recharge/recharge.html',
	},

	icon7: {
		title: '缴费历史',
		iconUrl: 'http://clientdispatch.10086.cn:8080/group2/M00/00/6A/rBCJYlcCkb6AFlxkAAAGFPhkIFc135.png',

		type: 1,
		actionUrl: '',
	},

	icon8: {
		title: '积分明细',
		iconUrl: 'http://clientdispatch.10086.cn:8080/group1/M00/00/62/rBCJYFcCkgOANED6AAAFxh3dEwQ300.png',

		type: 1,
		actionUrl: '',
	},
}


var styles = StyleSheet.create({

  container: {
    flex: 1,
    flexDirection: 'column',
    justifyContent: 'flex-start',
    // alignItems: 'center',
  },

  wrapper: {
  	// flex:1,
  	// height: 120,
  },
  slide: {
    flex: 1,
    backgroundColor: 'transparent',
  },
  image: {
    flex: 1,
    // resizeMode: Image.resizeMode.contain,
    // borderColor: 'green',
    // borderWidth: 2,

    // width: 400,
    // height:160,
  },

  iconList: {
  	// flex:1,
  	flexDirection: 'row',
  	flexWrap: 'wrap',
  	justifyContent: 'space-between',
  	alignItems: 'flex-start',
  	// alignSelf: 'center',

  	// height: 180,
  	// backgroundColor: 'red',
  },

  iconButton: {
  	// flex:1,
  	marginLeft: 20,
  	marginTop: 20,
  	marginRight: 20,
  	marginBottom: 10,

  	width: 44,
  	height: 44,
  	// backgroundColor: 'blue',
  },
});


class IconButton extends Component {

	iconPressAction() {

		var page = SecondView;
		if (this.props.args.type == 2) {
			//action url
			const url = this.props.args.actionUrl;
			page = LDWebViewPage;
			// page = <LDWebViewPage url={url}/>;
		}

		this.props.nav.push({args: {url: this.props.args.actionUrl, title: this.props.args.title}, component: page});
	}

	render() {

		return (

				<TouchableOpacity onPress={this.iconPressAction.bind(this)}>
				  <View style={{alignItems: 'center', justifyContent: 'flex-start', }}>
					<Image style={styles.iconButton} source={{uri: this.props.args.iconUrl}}/>
					<Text> {this.props.args.title} </Text>
				  </View>
				</TouchableOpacity>
			);
	}
}

class TabItemPage extends Component {


  pressAction() {

      //每次调用push时，其实就是调用MyNavigator中的renderScene（_renderScene）
      this.props.nav.push({
          component: SecondView,
        });
  }

  bannerAction(e, state, context) {

  	console.log(e, state, context.state);
  }

  render() {

    return (
        <View style={styles.container} backgroundColor={this.props.color}>

	        <Swiper style={styles.wrapper} autoplay={true} height={140} >

				  <View style={styles.slide}>
		            <Image style={styles.image} source={{uri: ImageRes.pic1}} />
		          </View>
				  <View style={styles.slide}>
		            <Image style={styles.image} source={{uri: ImageRes.pic2}} />
		          </View>
				  <View style={styles.slide}>
		            <Image style={styles.image} source={{uri: ImageRes.pic3}} />
		          </View>
				  <View style={styles.slide}>
		            <Image style={styles.image} source={{uri: ImageRes.pic4}} />
		          </View>

			  </Swiper>

			  <View style={styles.iconList}>
			  		<IconButton nav={this.props.nav} args={ImageRes.icon1}/>
			  		<IconButton nav={this.props.nav} args={ImageRes.icon2}/>
			  		<IconButton nav={this.props.nav} args={ImageRes.icon3}/>
			  		<IconButton nav={this.props.nav} args={ImageRes.icon4}/>

			  		<IconButton nav={this.props.nav} args={ImageRes.icon5}/>
			  		<IconButton nav={this.props.nav} args={ImageRes.icon6}/>
			  		<IconButton nav={this.props.nav} args={ImageRes.icon7}/>
			  		<IconButton nav={this.props.nav} args={ImageRes.icon8}/>
			  </View>
			  <View style={{flex:1, marginTop: 20, backgroundColor:'red'}}>
				  <WebView
			          ref={'webView'}
			          automaticallyAdjustContentInsets={false}
			          style={styles.webView}
			          source={{uri: 'http://www.baidu.com'}}
			          javaScriptEnabled={true}
			          domStorageEnabled={true}
			          decelerationRate="normal"
			          onNavigationStateChange={this.onNavigationStateChange}
			          onShouldStartLoadWithRequest={this.onShouldStartLoadWithRequest}
			          startInLoadingState={true}
			          scalesPageToFit={true}/>
			  </View>
        </View>
      );
  }
}

module.exports = TabItemPage;