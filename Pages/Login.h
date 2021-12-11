﻿#pragma once

#include "Login.g.h"
#include "MainPage.h"

namespace winrt::bikabika::implementation
{
    struct Login : LoginT<Login>
    {
        Login();
        Windows::Foundation::IAsyncAction ReadAccountJson();

        Windows::Foundation::IAsyncAction WriteAccountJson(hstring email, hstring password, hstring token, boolean isCheck);

        Windows::Foundation::IAsyncAction LoginAccount();

        Windows::Foundation::IAsyncAction SetPerson();

        bikabika::BikaHttp BikaHttpAPI();
        //Windows::Foundation::IAsyncOperation<winrt::hstring>  ReadAccountJson();

        void LoginClickHandler(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
        void Password_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        Windows::Foundation::IAsyncAction AutoLogin();
    public:
        bikabika::BikaHttp m_bikaHttp;
        bikabika::FileCheckTool m_fileCheckTool;
    };
}

namespace winrt::bikabika::factory_implementation
{
    struct Login : LoginT<Login, implementation::Login>
    {
    };
}