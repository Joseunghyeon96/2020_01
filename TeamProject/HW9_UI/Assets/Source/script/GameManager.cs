using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{
    public static bool bCheck_Parameter = true;

    public static int bHardware = 1;

    [SerializeField] private InputField Name_Input;
    [SerializeField] private InputField Password_Input;
    [SerializeField] private InputField Search_Input;
    [SerializeField] private Slider Slider_Value;



    // Start is called before the first frame update
    void Start()
    {
        bHardware = 1;
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void MyButtonScript()
    {
        Debug.Log("My button script fuction");
    }

    public void MyCheckButtonScript()
    {
        bCheck_Parameter = !bCheck_Parameter;

        Debug.Log("My check button :" + bCheck_Parameter);
    }

    public void MyRadioButtonScript1()
    {
        if (bHardware == 1) return;
        bHardware = 1;
        Debug.Log("My radio button script1 fuction . bHardware :" + bHardware);
    }
    public void MyRadioButtonScript2()
    {
        if (bHardware == 2) return;
        bHardware = 2;
        Debug.Log("My radio button script2 fuction . bHardware :" + bHardware);
    }

    public void MyInputField_Name()
    {
        Debug.Log("ID를 입력했습니다.");
    }

    public void MyInputField_Password()
    {
        Debug.Log("암호를 입력합니다");
    }
    
    public void MyInputField_Search()
    {
        Debug.Log("검색어를 입력합니다.");
    }
    public void ClickSearch()
    {
        Debug.Log(Search_Input.text + "을 검색합니다!");
    }

    public void ClickLogin()
    {
        if (Name_Input.text == "조승현")
        {
            Debug.Log("존재하는 ID입니다!");
            if (Password_Input.text == "1234")
                Debug.Log("암호가 일치합니다. 로그인합니다!");

            else
                Debug.Log("패스워드가 일치하지 않습니다!");
        }
        else
        {
            Debug.Log("존재하지 않는 ID입니다.");
            return;
        }
    }
    public void MySlider()
    {
        Debug.Log("슬라이더값 :" + Slider_Value.value);
    }
}
