﻿#include "pch.h"

#include "MainPage.h"

#include "MainPage.g.cpp"
using namespace winrt::Windows::UI::Xaml::Media;




using namespace std;
using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Numerics;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::ApplicationModel::Core;

namespace winrt::bikabika::implementation
{
	bikabika::MainPage MainPage::current{ nullptr };
	
	MainPage::MainPage()
	{
		
		InitializeComponent();
		Windows::Storage::ApplicationDataContainer serversSettings = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"Servers", Windows::Storage::ApplicationDataCreateDisposition::Always);
		Windows::Storage::ApplicationDataContainer userData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"User", Windows::Storage::ApplicationDataCreateDisposition::Always);
		Windows::Storage::ApplicationDataContainer settingsData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"Settings", Windows::Storage::ApplicationDataCreateDisposition::Always);
		if (!serversSettings.Values().HasKey(L"picServer1")) serversSettings.Values().Insert(L"picServer1", box_value(L"https://storage1.picacomic.com"));
		if (!serversSettings.Values().HasKey(L"picServer2")) serversSettings.Values().Insert(L"picServer2", box_value(L"https://s2.picacomic.com"));
		if (!serversSettings.Values().HasKey(L"picServer3")) serversSettings.Values().Insert(L"picServer3", box_value(L"https://s3.picacomic.com"));
		if (!serversSettings.Values().HasKey(L"picServersCurrent")) serversSettings.Values().Insert(L"picServersCurrent", box_value(L"https://storage1.picacomic.com"));
		if (!serversSettings.Values().HasKey(L"imageQuality")) settingsData.Values().Insert(L"imageQuality", box_value(L"original"));
		
		MainPage::current = *this;
		
			// 隐藏标题栏 
		auto coreTitleBar = CoreApplication::GetCurrentView().TitleBar();
		coreTitleBar.ExtendViewIntoTitleBar(true);
		
		Window::Current().SetTitleBar(CustomDragRegion());
		
		LoginTeachingTip().IsOpen(true);
		NavHome().IsEnabled(false);
		NavClassification().IsEnabled(false);
		NavAccount().IsEnabled(false);
		
	}

	void MainPage::NavView_ItemInvoked(
		Windows::Foundation::IInspectable const& /* sender */,
		muxc::NavigationViewItemInvokedEventArgs const& args)
	{
		if (args.IsSettingsInvoked())
		{
			NavView_Navigate(L"settings", args.RecommendedNavigationTransitionInfo());
		}
		else if (args.InvokedItemContainer())
		{
			NavView_Navigate(
				winrt::unbox_value_or<winrt::hstring>(
					args.InvokedItemContainer().Tag(), L"").c_str(),
				args.RecommendedNavigationTransitionInfo());
		}
	}


	
	void MainPage::NavView_Navigate(std::wstring navItemTag,Windows::UI::Xaml::Media::Animation::NavigationTransitionInfo const& transitionInfo)
	{
		Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
		winrt::Microsoft::UI::Xaml::Controls::TabViewItem newItem;
		winrt::Microsoft::UI::Xaml::Controls::SymbolIconSource symbol;
		winrt::Windows::UI::Xaml::Controls::Frame frame;

		if (navItemTag == L"settings")
		{
			newItem.Header(box_value(resourceLoader.GetString(L"NavSettings/Content")));
			symbol.Symbol(Symbol::Setting);
			frame.Navigate(winrt::xaml_typename<bikabika::SettingsPage>());
		}
		else if (navItemTag == L"home")
		{
			frame.Navigate(winrt::xaml_typename<bikabika::HomePage>());
			newItem.Header(box_value(resourceLoader.GetString(L"NavHome/Content")));
			symbol.Symbol(Symbol::Home);
		}
		else if (navItemTag == L"classification")
		{
			frame.Navigate(winrt::xaml_typename<bikabika::ClassificationPage>());
			newItem.Header(box_value(resourceLoader.GetString(L"NavClassification/Content")));
			symbol.Symbol(Symbol::AllApps);
		}
		else if (navItemTag == L"account")
		{
			frame.Navigate(winrt::xaml_typename<bikabika::UserPage>());
			newItem.Header(box_value(resourceLoader.GetString(L"NavAccount/Content")));
			symbol.Symbol(Symbol::Contact);
		}
		else if (navItemTag == L"download")
		{
			frame.Navigate(winrt::xaml_typename<bikabika::DownloadPage>());
			newItem.Header(box_value(resourceLoader.GetString(L"NavHome/Content")));
			symbol.Symbol(Symbol::Home);
		}
		else return;
		newItem.IconSource(symbol);
		newItem.Content(frame);
		ContentTabView().TabItems().Append(newItem);
		ContentTabView().SelectedItem(newItem);
		NavView().SelectedItem(NavView().MenuItems().GetAt(0));
		
	}
	void MainPage::CreateNewTab(Windows::UI::Xaml::Controls::Frame const& frame, hstring const& title,Microsoft::UI::Xaml::Controls::SymbolIconSource const& symbol)
	{
		
		winrt::Microsoft::UI::Xaml::Controls::TabViewItem newItem;
		newItem.Header(box_value(title));
		newItem.IconSource(symbol);
		newItem.Content(frame);
		Dispatcher().RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, [newItem, this]()
			{
				ContentTabView().TabItems().Append(newItem);
				ContentTabView().SelectedItem(newItem);
				NavView().SelectedItem(NavView().MenuItems().GetAt(0));
			});
		
	}

	void MainPage::ContentFrame_NavigationFailed(
		Windows::Foundation::IInspectable const& /* sender */,
		Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const& args)
	{
		throw winrt::hresult_error(
			E_FAIL, winrt::hstring(L"Failed to load Page ") + args.SourcePageType().Name);
	}

	void MainPage::On_Navigated(
		Windows::Foundation::IInspectable const& /* sender */,
		Windows::UI::Xaml::Navigation::NavigationEventArgs const& args)
	{
		//NavView().IsBackEnabled(ContentFrame().CanGoBack());

		//if (ContentFrame().SourcePageType().Name ==
		//	winrt::xaml_typename<bikabika::SettingsPage>().Name)
		//{
		//	// SettingsItem is not part of NavView.MenuItems, and doesn't have a Tag.
		//	NavView().SelectedItem(NavView().SettingsItem().as<muxc::NavigationViewItem>());
		//	NavView().Header(winrt::box_value(L"Settings"));
		//}
		//else if (ContentFrame().SourcePageType().Name != L"")
		//{
		//	//m_userViewModel.User().Img(winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage{ Windows::Foundation::Uri{ L"https://storage1.picacomic.com/static/0788a77a-81e4-46a5-9206-c424226bed07.jpg" } });
		//	for (auto&& eachPage : m_pages)
		//	{
		//		if (eachPage.second.Name == args.SourcePageType().Name)
		//		{
		//			for (auto&& eachMenuItem : NavView().MenuItems())
		//			{
		//				auto navigationViewItem =
		//					eachMenuItem.try_as<muxc::NavigationViewItem>();
		//				{
		//					if (navigationViewItem)
		//					{
		//						winrt::hstring hstringValue =
		//							winrt::unbox_value_or<winrt::hstring>(
		//								navigationViewItem.Tag(), L"");
		//						if (hstringValue == eachPage.first)
		//						{
		//							NavView().SelectedItem(navigationViewItem);
		//							NavView().Header(navigationViewItem.Content());
		//						}
		//					}
		//				}
		//			}
		//			break;
		//		}
		//	}
		//}
	}

	void MainPage::ContentFrame_Navigated(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
	{
		//bug Comics 会自动弹出搜索框
	}

	bikabika::UserViewModel MainPage::MainUserViewModel()
	{
		return m_userViewModel;
	}

	//todo 划分为标签页

	/*
	* void MainPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
	{
		auto propertyValue{ e.Parameter().as<Windows::Foundation::IPropertyValue>() };
		if (propertyValue.Type() == Windows::Foundation::PropertyType::String)
		{
			auto ss = winrt::unbox_value<winrt::hstring>(e.Parameter());
			Windows::Data::Json::JsonObject personData = Windows::Data::Json::JsonObject::Parse(ss);
			Uri url(L"https://storage1.picacomic.com/static/" + personData.GetNamedObject(L"avatar").GetNamedString(L"path"));
			Windows::UI::Xaml::Media::Imaging::BitmapImage source(url);
			UsersPic().ProfilePicture(source);
		}
		else
		{
			__super::OnNavigatedTo(e);
		}

	}
	Windows::Foundation::IAsyncAction MainPage::CreateLoginPage(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
	{	// 新建页面

		auto Views = CoreApplication::Views();
		if (Views.Size() == 2)
			return;
		CoreDispatcher MainDispatcher =
			CoreApplication::GetCurrentView().Dispatcher();
		int32_t  ViewId = 0;
		// Create a new window.

		CoreApplicationView View = CoreApplication::CreateNewView();
		co_await resume_foreground(View.Dispatcher());

		// Show the "Loading..." screen.
		Windows::UI::Xaml::Controls::Frame f;
		f.Navigate(xaml_typename<bikabika::LoginPage>(), nullptr);
		Window::Current().Content(f);
		Window::Current().Activate();

		//ApplicationView::PreferredLaunchViewSize();
		//ApplicationView::PreferredLaunchWindowingMode(ApplicationViewWindowingMode::Auto);
		//Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().TryResizeView(Size(600, 400));

		ViewId = ApplicationView::GetForCurrentView().Id();

		// Activate the new window.
		co_await resume_foreground(MainDispatcher);

		co_await ApplicationViewSwitcher::TryShowAsStandaloneAsync(ViewId);

		// Start the emulation in the new window.
		co_await resume_foreground(View.Dispatcher());
		co_await ApplicationViewSwitcher::SwitchAsync(ViewId);

		//ApplicationView::GetForCurrentView().ExitFullScreenMode();
	}*/

	// 搜索栏
	void  winrt::bikabika::implementation::MainPage::CatSearch_TextChanged(winrt::Windows::UI::Xaml::Controls::AutoSuggestBox const& sender, winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxTextChangedEventArgs const& args)
	{
		if (sender.Text()!=L""&&(args.Reason() == AutoSuggestionBoxTextChangeReason::UserInput || (args.Reason() == AutoSuggestionBoxTextChangeReason::ProgrammaticChange && sender.Text() == L" ")))
		{
			if (!m_suggestIsChosen)
			{
				sender.ItemsSource(box_value(m_suggestions));
			}
			m_suggestIsChosen = false;
		}
	}
	// 搜索栏提交
	void winrt::bikabika::implementation::MainPage::CatSearch_QuerySubmitted(winrt::Windows::UI::Xaml::Controls::AutoSuggestBox const& sender, winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxQuerySubmittedEventArgs const& args)
	{
		extern bool loadComicFlag;
		loadComicFlag = true;
		auto text = sender.Text();
		if (text != L"")
		{
			Windows::Storage::ApplicationDataContainer userData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"User", Windows::Storage::ApplicationDataCreateDisposition::Always);
			
			if (userData.Values().HasKey(L"SearchHistorys"))
			{
				int i;
				bool f = false;
				auto y = Windows::Data::Json::JsonArray::Parse(unbox_value<hstring>(userData.Values().Lookup(L"SearchHistorys")));
				for (i = 0; i < y.Size(); i++) 
				{
					if (text == y.GetStringAt(i))
					{
						f = true;
						break;
					}
				}
				if(f)
				{
					y.RemoveAt(i);
					m_suggestions.RemoveAt(i);
				}
				y.InsertAt(0, Windows::Data::Json::JsonValue::CreateStringValue(text));
				userData.Values().Insert(L"SearchHistorys", box_value(y.Stringify()));
				m_suggestions.InsertAt(0, winrt::make<KeywordsBox>(text, L"历史记录", L"[TAG]", winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage(winrt::Windows::Foundation::Uri(L"ms-appx:///Assets//Picacgs//history.png"))));

				
			}
			else {
				Windows::Data::Json::JsonArray json;
				userData.Values().Insert(L"SearchHistorys", box_value(json.Stringify()));
			}
			ComicArgs args;
			args.ComicType(ComicsType::SEARCH);
			args.Content(text);
			args.SortMode(winrt::bikabika::SearchSortMode::DD);
			sender.ItemsSource(box_value(winrt::single_threaded_observable_vector<bikabika::KeywordsBox>()));
			Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
			winrt::Microsoft::UI::Xaml::Controls::TabViewItem newItem;
			newItem.Header(box_value(sender.Text()));
			winrt::Microsoft::UI::Xaml::Controls::SymbolIconSource symbol;
			symbol.Symbol(Symbol::List);
			newItem.IconSource(symbol);
			winrt::Windows::UI::Xaml::Controls::Frame frame;
			frame.Navigate(winrt::xaml_typename<bikabika::ComicsPage>(), box_value(args));
			newItem.Content(frame);
			ContentTabView().TabItems().Append(newItem);
			ContentTabView().SelectedItem(newItem);
			SearchFlyout().Hide();
		}
		sender.Text(L"");
		//ClearCatSearchHistory().Visibility(Visibility::Collapsed);
	}

	
	void winrt::bikabika::implementation::MainPage::CatSearch_SuggestionChosen(winrt::Windows::UI::Xaml::Controls::AutoSuggestBox const& sender, winrt::Windows::UI::Xaml::Controls::AutoSuggestBoxSuggestionChosenEventArgs const& args)
	{
		m_suggestIsChosen = true;
		hstring s = args.SelectedItem().as<bikabika::KeywordsBox>().GetKeywords();
		sender.Text(s);
	}
	void MainPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs const& e)
	{
		__super::OnNavigatedTo(e);
	}
	// 弹出窗
	Windows::Foundation::IAsyncAction  MainPage::ContentDialogShow(hstring const& mode, hstring const& message)
	{
		
		Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
		if (mode == L"Timeout") {
			auto show{ PicErrorDialog().ShowAsync() };
		}
		else {
			HttpContentDialog().Title(box_value(resourceLoader.GetString(L"FailHttpTitle")));
			HttpContentDialog().CloseButtonText(resourceLoader.GetString(L"FailCloseButtonText"));
			if (mode == L"Error")
			{
				HttpContentDialog().Content(box_value(message));

				auto show{ co_await HttpContentDialog().ShowAsync() };

			}
			else if (mode == L"Unknown")
			{
				Windows::Data::Json::JsonObject resp = Windows::Data::Json::JsonObject::Parse(message);
				HttpContentDialog().Content(box_value(to_hstring(resp.GetNamedNumber(L"code")) + L":" + resp.GetNamedString(L"message")));
				HttpContentDialog().IsTextScaleFactorEnabled(true);
				auto show{ co_await HttpContentDialog().ShowAsync() };

			}
			else if (mode == L"1005")
			{
				HttpContentDialog().Content(box_value(resourceLoader.GetString(L"FailAuth")));
				HttpContentDialog().IsTextScaleFactorEnabled(true);
				extern bool m_login;
				m_login = false;
				auto show{ co_await HttpContentDialog().ShowAsync() };
				if (show == winrt::Windows::UI::Xaml::Controls::ContentDialogResult::None)
				{
					auto loginTeachingTip = Frame().Parent().as<Microsoft::UI::Xaml::Controls::NavigationView>().Parent().as<Windows::UI::Xaml::Controls::Grid>().Children().GetAt(3).as<Microsoft::UI::Xaml::Controls::TeachingTip>();
					loginTeachingTip.Title(resourceLoader.GetString(L"LoginButton/Content"));
					loginTeachingTip.IsOpen(true);
				}
			}
			else if (mode == L"Blank")
			{
				HttpContentDialog().Content(box_value(resourceLoader.GetString(L"FailLoginBlank")));
				auto show{ HttpContentDialog().ShowAsync() };
			}
			
		}
	}
	Windows::Foundation::IAsyncAction MainPage::Login()
	{
		
		auto ress = co_await m_bikaHttp.Login(Email().Text(), Password().Password());
		LayoutMessage().IsOpen(false);
		if (ress[1] == 'T')
		{
			ContentDialogShow(L"Timeout", L"");
		}
		else if (ress[1] == 'E') {
			ContentDialogShow(L"Error", ress);
		}
		else
		{
			Windows::Data::Json::JsonObject resp = Windows::Data::Json::JsonObject::Parse(ress);
			double code = resp.GetNamedNumber(L"code");
			//账号或密码错误
			if (code == (double)400)
			{
				ContentDialogShow(L"LoginError", L"");
				Password().Password(L"");
			}
			//登陆成功
			else if (code == (double)200)
			{
				Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
				loginData.Values().Insert(L"token", box_value(resp.GetNamedObject(L"data").GetNamedString(L"token")));
				loginData.Values().Insert(L"email", box_value(Email().Text()));
				loginData.Values().Insert(L"password", box_value(Password().Password()));
				Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
				LayoutMessage().Title(resourceLoader.GetString(L"Logining"));
				LayoutMessage().IsOpen(true);
				NavHome().IsEnabled(true);
				NavClassification().IsEnabled(true);
				NavAccount().IsEnabled(true);
				co_await SetPerson();
				
			}
			//未知
			else
			{
				ContentDialogShow(L"Unknown", ress);
			}
		}

	}
	Windows::Foundation::IAsyncAction MainPage::SetPerson()
	{
		auto resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
		hstring personInfo = co_await m_bikaHttp.PersonInfo();
		LayoutMessage().IsOpen(false);
		if (personInfo[1] == 'T') {
			ContentDialogShow(L"Timeout", L"");
		}
		else if (personInfo[1] == 'E') {
			ContentDialogShow(L"Error", personInfo);
		}
		else {
			Windows::Data::Json::JsonObject personData = Windows::Data::Json::JsonObject::Parse(personInfo);
			double code = personData.GetNamedNumber(L"code");
			//缺少鉴权
			if (code == (double)401 && personData.GetNamedString(L"error") == L"1005")
			{
				co_await Login();
				//ContentDialogShow(L"1005", L"");
			}
			//请求无效
			else if (code == (double)400)
			{
				ContentDialogShow(L"Error", personInfo);
			}
			else if (code == (double)200)
			{
				Windows::Storage::ApplicationDataContainer userData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"User", Windows::Storage::ApplicationDataCreateDisposition::Always);
				auto personInfo = personData.GetNamedObject(L"data").GetNamedObject(L"user");
				userData.Values().Insert(L"personInfo", box_value(personInfo.Stringify()));
				LoginTeachingTip().IsOpen(false);
				extern bool m_login;
				m_login = true;
				Windows::Storage::ApplicationDataContainer serversSettings = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"Servers", Windows::Storage::ApplicationDataCreateDisposition::Always);
				if (userData.Values().HasKey(L"personInfo"))
				{
					Windows::Data::Json::JsonObject personInfo = winrt::Windows::Data::Json::JsonObject::Parse(unbox_value<winrt::hstring>(userData.Values().TryLookup(L"personInfo")));
					hstring name = personInfo.GetNamedString(L"name");
					hstring slogan = L"\"" + to_hstring(personInfo.GetNamedString(L"slogan")) + L"\"";
					m_userViewModel.User().Slogan(slogan);
					if (name != m_userViewModel.User().Name()) m_userViewModel.User().Name(name);
					hstring level = L"Lv." + to_hstring(personInfo.GetNamedNumber(L"level"));
					if (level != m_userViewModel.User().Level()) m_userViewModel.User().Level(level);
					hstring img = unbox_value<winrt::hstring>(serversSettings.Values().Lookup(L"picServersCurrent")) + L"/static/" + personInfo.GetNamedObject(L"avatar").GetNamedString(L"path");
					if (img != m_userViewModel.User().Img().UriSource().ToString()) {
						m_userViewModel.User().Img(winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage{ Windows::Foundation::Uri{ img} });
					}
				}
				NavHome().IsEnabled(true);
				NavClassification().IsEnabled(true);
				NavAccount().IsEnabled(true);
				if (m_firstArrive) {
					Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
					winrt::Microsoft::UI::Xaml::Controls::TabViewItem newItem;
					newItem.Header(box_value(resourceLoader.GetString(L"NavHome/Content")));
					winrt::Microsoft::UI::Xaml::Controls::SymbolIconSource symbol;
					symbol.Symbol(Symbol::Home);
					newItem.IconSource(symbol);
					winrt::Windows::UI::Xaml::Controls::Frame frame;
					frame.Navigate(winrt::xaml_typename<bikabika::HomePage>());
					newItem.Content(frame);
					ContentTabView().TabItems().Append(newItem);
					ContentTabView().SelectedItem(newItem);
					m_firstArrive = false;
					co_await GetKeywords();
				}
				
			}
			//未知
			else
			{
				ContentDialogShow(L"Unknown", personInfo);
			}
		}

	}
	//登录按钮
	void MainPage::LoginClickHandler(Windows::Foundation::IInspectable const& /*sender*/ , Windows::UI::Xaml::RoutedEventArgs const& /*args*/)
	{
		// 账号密码为空
		if (Email().Text() == L"" || Password().Password() == L"")
		{
			ContentDialogShow(L"Blank", L"");
		}
		else
		{
			Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
			LayoutMessage().Title(resourceLoader.GetString(L"Logining"));
			LayoutMessage().IsOpen(true);
			auto login{ Login() };
		}
	}
	Windows::Foundation::IAsyncAction MainPage::GetKeywords()
	{   
		hstring res{ co_await m_bikaHttp.Keywords() };
		if (res[1] == 'T')
		{
			ContentDialogShow(L"Timeout", L"");
		}
		else if (res[1] == 'E') {
			ContentDialogShow(L"Error", res);
		}
		else
		{
			Windows::Data::Json::JsonObject resp = Windows::Data::Json::JsonObject::Parse(res);
			double code = resp.GetNamedNumber(L"code");
			if (code == (double)200)
			{
				Windows::Storage::ApplicationDataContainer userData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"User", Windows::Storage::ApplicationDataCreateDisposition::Always);
				if (userData.Values().HasKey(L"SearchHistorys"))
				{
					auto y = Windows::Data::Json::JsonArray::Parse(unbox_value<hstring>(userData.Values().Lookup(L"SearchHistorys")));
					
					for (auto z : y)
					{
						OutputDebugStringW(z.GetString().c_str());
						OutputDebugStringW(L"\n");
						m_suggestions.Append(winrt::make<KeywordsBox>(z.GetString(), L"历史记录", L"[TAG]", winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage(winrt::Windows::Foundation::Uri(L"ms-appx:///Assets//Picacgs//history.png"))));
					}
				}
				else {
					Windows::Data::Json::JsonArray json;
					userData.Values().Insert(L"SearchHistorys", box_value(json.Stringify()));
					OutputDebugStringW(L"[NEW]SearchHistorys\n");
				}
				auto keywords = resp.GetNamedObject(L"data").GetNamedArray(L"keywords");
				userData.Values().Insert(L"keywords", box_value(keywords.Stringify()));
				m_suggestionSize = 0;
				for (auto x : keywords)
				{
					m_suggestionSize++;
					m_suggestions.Append(winrt::make<KeywordsBox>(x.GetString(), L"大家都在搜", L"[TAG]", winrt::Windows::UI::Xaml::Media::Imaging::BitmapImage(winrt::Windows::Foundation::Uri(L"ms-appx:///Assets//Picacgs//tag.png"))));
				}
			}
			//未知
			else
			{
				ContentDialogShow(L"Unknown", res);
			}
		}
	}
	void winrt::bikabika::implementation::MainPage::Password_Loaded(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Windows::UI::Xaml::RoutedEventArgs const& /*e*/)
	{

		Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
		if (auto x = loginData.Values().TryLookup(L"email"))
		{
			Email().Text(unbox_value<hstring>(x));
		}
		
		if (auto y = loginData.Values().TryLookup(L"rememberMe"))
		{
			if (unbox_value<bool>(y))
			{
				RememberCheckBox().IsChecked(unbox_value<bool>(y));
				if (auto z = loginData.Values().TryLookup(L"password"))
				{
					Password().Password(unbox_value<hstring>(z));
				}
				if (auto s = loginData.Values().TryLookup(L"autoLogin"))
				{
					
					AutoCheckBox().IsChecked(unbox_value<bool>(s));

				}
			}
			else
			{
				Password().Password(L"");
			}
			
		}
		
		auto bcc{ AutoLogin() };
	}
	hstring MainPage::PicPath()
	{
		return m_picPath;
	}
	//自动登录
	Windows::Foundation::IAsyncAction MainPage::AutoLogin()
	{
		Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
		if (auto x = loginData.Values().TryLookup(L"autoLogin"))
		{
			if (unbox_value<bool>(x))
			{
				Windows::Data::Json::JsonObject account{ co_await m_fileCheckTool.GetAccount() };
				if (auto s = loginData.Values().TryLookup(L"rememberMe"))
				{
					if (unbox_value<bool>(s))
					{
						Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
						LayoutMessage().Title(resourceLoader.GetString(L"AutoLogining"));
						LayoutMessage().IsOpen(true);
						co_await SetPerson();
					}
				}

			}
		}
	}
	void winrt::bikabika::implementation::MainPage::UsersPic_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Input::PointerRoutedEventArgs const& e)
	{
		extern bool m_login;
		if (m_login) {
			menu().ShowAt(sender.as<FrameworkElement>());
		}
		else {
			LoginTeachingTip().IsOpen(true);
		}
		
		/*extern bool m_login;
		if (m_login)
		{
			
			Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
			winrt::Microsoft::UI::Xaml::Controls::TabViewItem newItem;
			newItem.Header(box_value(resourceLoader.GetString(L"NavAccount/Content")));
			winrt::Microsoft::UI::Xaml::Controls::SymbolIconSource symbol;
			symbol.Symbol(Symbol::Contact);
			newItem.IconSource(symbol);
			winrt::Windows::UI::Xaml::Controls::Frame frame;
			frame.Navigate(winrt::xaml_typename<bikabika::UserPage>());
			newItem.Content(frame);
			ContentTabView().TabItems().Append(newItem);
			ContentTabView().SelectedItem(newItem);
		}
		else
		{
			Windows::ApplicationModel::Resources::ResourceLoader resourceLoader{ Windows::ApplicationModel::Resources::ResourceLoader::GetForCurrentView() };
			LoginTeachingTip().Title(resourceLoader.GetString(L"LoginButton/Content"));
			LoginTeachingTip().IsOpen(true);
		}*/
		
	}

}














void winrt::bikabika::implementation::MainPage::AutoCheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	if (AutoCheckBox().IsChecked().GetBoolean())
	{
		RememberCheckBox().IsChecked(true);
	}
	Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
	loginData.Values().Insert(L"autoLogin", box_value(RememberCheckBox().IsChecked().GetBoolean()));
}


void winrt::bikabika::implementation::MainPage::RememberCheckBox_Checked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	
	Windows::Storage::ApplicationDataContainer loginData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"LoginData", Windows::Storage::ApplicationDataCreateDisposition::Always);
	loginData.Values().Insert(L"rememberMe", box_value(RememberCheckBox().IsChecked().GetBoolean()));
}



void winrt::bikabika::implementation::MainPage::ClearCatSearchHistory_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e)
{
	Windows::Storage::ApplicationDataContainer userData = Windows::Storage::ApplicationData::Current().LocalSettings().CreateContainer(L"User", Windows::Storage::ApplicationDataCreateDisposition::Always);
	Windows::Data::Json::JsonArray json;
	userData.Values().Insert(L"SearchHistorys", box_value(json.Stringify()));
	int clearAll = m_suggestions.Size() - m_suggestionSize;
	for(int i=0;i< clearAll;i++)
	{
		m_suggestions.RemoveAt(0);
	}

}


void winrt::bikabika::implementation::MainPage::ContentTabView_TabCloseRequested(winrt::Microsoft::UI::Xaml::Controls::TabView const& sender, winrt::Microsoft::UI::Xaml::Controls::TabViewTabCloseRequestedEventArgs const& args)
{
	uint32_t tabIndexFromControl;
	if(sender.TabItems().IndexOf(args.Tab(), tabIndexFromControl))
	{
		sender.TabItems().RemoveAt(tabIndexFromControl);
	}
	
	
}



void winrt::bikabika::implementation::MainPage::Grid_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::SizeChangedEventArgs const& e)
{
	ContentTabView().Width(sender.as<Controls::Frame>().ActualWidth());
	
}


void winrt::bikabika::implementation::MainPage::NavView_PaneClosed(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Windows::Foundation::IInspectable const& args)
{
	ContentFrame().Margin(Thickness{ 53,0,0,0 });
	APPTitle().Visibility(Visibility::Collapsed);
	NavSmallImg().Visibility(Visibility::Visible);
	NavBigImg().Visibility(Visibility::Collapsed);
}


void winrt::bikabika::implementation::MainPage::NavView_PaneOpened(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Windows::Foundation::IInspectable const& args)
{
	ContentFrame().Margin(Thickness{ 185,0,0,0 });
	APPTitle().Visibility(Visibility::Visible);
	NavSmallImg().Visibility(Visibility::Collapsed);
	NavBigImg().Visibility(Visibility::Visible);
}


void winrt::bikabika::implementation::MainPage::Flyout_Opened(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e)
{
	CatSearch().Text(L" ");
	CatSearch().ItemsSource(box_value(m_suggestions));
	CatSearch().Text(L"");
}
