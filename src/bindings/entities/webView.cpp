#include "Log.h"
#include "../../helpers/module.h"
#include "entity.h"
#include "../data/vector2.h"

using namespace Helpers;
using namespace Data;
/*
#ifdef CLIENT_MODULE
static std::string ToString(alt::IWebView* view)
{
    std::stringstream str;
    str << "WebView{ url: " << view->GetUrl() << " }";
    return str.str();
}

static alt::IWebView* WebviewFactory(const std::string& url, bool overlay = false, Vector2& pos = { 0, 0 }, Vector2& size = { 0, 0 })
{
    GET_RESOURCE();
    auto webview = alt::ICore::Instance().CreateWebView(resource->GetIResource(), url, { pos.x, pos.y }, { size.x, size.y }, true, overlay);
    AS_ASSERT(!webview.IsEmpty(), "Failed to create webview", nullptr);

    webview->AddRef();
    return webview.Get();
}
static alt::IWebView* WebviewFactory3D(const std::string& url, uint32_t drawable, const std::string& texture)
{
    GET_RESOURCE();
    auto webview = alt::ICore::Instance().CreateWebView(resource->GetIResource(), url, drawable, texture);
    AS_ASSERT(!webview.IsEmpty(), "Failed to create webview", nullptr);

    webview->AddRef();
    return webview.Get();
}

static ModuleExtension webviewExtension("alt", [](asIScriptEngine* engine, DocsGenerator* docs) {
    RegisterAsBaseObject<alt::IWebView>(engine, docs, "WebView");

    REGISTER_METHOD_WRAPPER("WebView", "string opImplConv() const", ToString);

    REGISTER_FACTORY("WebView", "const string&in url, bool overlay = false, Vector2&in pos = {0, 0}, Vector2&in size = {0, 0}", WebviewFactory);
    REGISTER_FACTORY("WebView", "const string&in url, Hash drawable, const string&in texture", WebviewFactory3D);
});
#endif
*/
