

import React, {Component} from 'react';
import {

	View,
	StyleSheet,
	Image,
	TabBarIOS,
} from 'react-native';

const TabItemPage = require('./View/TabbarItemPage');



class TabItems extends Component {

  constructor(props) {
    super(props);

    this.state = {
      
        selectedTab: 'HomePage',
        notifCount: 0,
        presses: 0,
    };

    this.statics = {
      title: '<TabBarIOS>',
      description: 'Tab-based navigation.',
    };

    this.displayName = 'TabBarExample';
  }

  _renderContent(color: string, pageText: string, num?: number) {
    return (
      <TabItemPage nav={this.props.nav} color={color} text={pageText}/>
    );
  }

  render() {

    return (

        <TabBarIOS
          tintColor="dodgerblue"
          barTintColor="white">

          <TabBarIOS.Item
            title="首页"
            icon={require('../res/png/home.png')}
            selected={this.state.selectedTab === 'HomePage'}
            onPress={() => {
              this.setState({
                selectedTab: 'HomePage',
              });
            }}>
            {this._renderContent('white', 'HomePage')}
          </TabBarIOS.Item>

          <TabBarIOS.Item
          title="我的"
          icon={require('../res/png/user.png')}
          badge={this.state.notifCount > 0 ? this.state.notifCount : undefined}
          selected={this.state.selectedTab === 'UserPage'}
          onPress={() => {
            this.setState({
              selectedTab: 'UserPage',
              notifCount: this.state.notifCount,
            });
          }}>
          {this._renderContent('white', 'UserPage', this.state.notifCount)}
          </TabBarIOS.Item>

          <TabBarIOS.Item
          icon={require('../res/png/business.png')}
          title="商城"
          badge={this.state.notifCount > 0 ? this.state.notifCount : undefined}
          selected={this.state.selectedTab === 'BusinessPage'}
          onPress={() => {
            this.setState({
              selectedTab: 'BusinessPage',
              notifCount: this.state.notifCount,
            });
          }}>
          {this._renderContent('white', 'BusinessPage', this.state.notifCount)}
          </TabBarIOS.Item>

          <TabBarIOS.Item
          icon={require('../res/png/service.png')}
          title="服务"
          badge={this.state.notifCount > 0 ? this.state.notifCount : undefined}
          selected={this.state.selectedTab === 'ServicePage'}
          onPress={() => {
            this.setState({
              selectedTab: 'ServicePage',
              notifCount: this.state.notifCount,
            });
          }}>
          {this._renderContent('white', 'ServicePage', this.state.notifCount)}
          </TabBarIOS.Item>

          <TabBarIOS.Item
            icon={require('../res/png/score.png')}
            title="积分"
            selected={this.state.selectedTab === 'ScorePage'}
            onPress={() => {
              this.setState({
                selectedTab: 'ScorePage',
                presses: this.state.presses,
              });
            }}>
            {this._renderContent('white', 'ScorePage', this.state.presses)}
          </TabBarIOS.Item>

        </TabBarIOS>

      );
  }
}

module.exports = TabItems;
