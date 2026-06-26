##
## EPITECH PROJECT, 2026
## zappy-mirror
## File description:
## streamlit
##

import pandas as pd
import json
import streamlit as st
import plotly.express as px

def open_file_log():
    """This function open the file log and recover json

    Returns:
        _type_: dataframe
    """
    with open("zappy_ai_streamlit.log") as file:
        data_json = []
        for lines in file:
            split_line = lines.split(" - INFO -", 1)
            data_json.append(json.loads(split_line[1]))
        dataset = pd.DataFrame(data_json)
        return dataset
  
datatset = open_file_log()

st.set_page_config(page_title="Zappy Dashboard 2026", layout="wide")

st.sidebar.title('Navigation')
pages = st.sidebar.radio("Menu", ["Home", "Data Analysis", "Authors"])

if (pages == "Home"):
    st.markdown("""<h1 style='color: #2E86C1; text-align: center;'>Welcome to the Zappy Dashboard</h1>""", unsafe_allow_html=True)
    st.subheader("Project summary")
    st.markdown("""<div style="background-color: #1e1e1e; padding: 20px; border-radius: 10px; border-left: 6px solid #2E86C1; border: 1px solid #333; color: #ffffff;">
        <h4 style="margin-top: 0; color: #5DADE2;"> About the Zappy project</h4> <p style="margin-bottom: 10px;"> The Zappy project involves implementing a server, a graphical interface, and functional autonomous agents..</p>
        <p style="margin-bottom: 5px;"><b> Why? Simply so that our agents reach <b>Level 8</b> as quickly as possible in a group of six.</p>
        <p style="margin-bottom: 0; color: #cccccc;">This Streamlit app allows you to explore the data and the evolution of the agents in depth. <i>Feel free to use the navigation bar on the left!</i></i></p>
        </div>""", unsafe_allow_html=True)
    st.markdown("""<h1 style='color: #2E86C1;'>General game information (zappy_ai__streamlit)</h1>""", unsafe_allow_html=True)
    st.markdown("---")
    st.subheader("Global statistics :")
    cols1, cols2, cols3, cols4 = st.columns(4)
    with cols1:
        st.markdown("<div style='text-align: center;'>Unique Players<br><span style='font-size: 2em; font-weight: bold; color: #2E86C1;'>{}</span></div>".format(len(datatset["id"].unique())), unsafe_allow_html=True)
    with cols2:
        st.markdown("<div style='text-align: center;'>Total Actions<br><span style='font-size: 2em; font-weight: bold; color: #ffcc00;'>{}</span></div>".format(len(datatset)), unsafe_allow_html=True)
    with cols3:
        st.markdown("<div style='text-align: center;'>Max Food (by one player)<br><span style='font-size: 2em; font-weight: bold; color: #2E86C1;'>{}</span></div>".format(datatset["food"].max()), unsafe_allow_html=True)
    with cols4:
        st.markdown("<div style='text-align: center;'>Max Level (by one player)<br><span style='font-size: 2em; font-weight: bold; color: #ffcc00;'>{}</span></div>".format(datatset["level"].max()), unsafe_allow_html=True)
    st.markdown("---")
    st.subheader("Match Result")
    info = datatset[datatset["level"] == 7]
    if not info.empty:
        st.markdown("![Alt Text](https://www.image2url.com/r2/default/gifs/1782413309071-bab97c21-b7bf-4db5-b9ac-cce52bee1660.gif)")
        winner = info["team"].iloc[0]
        st.markdown(f"""<div style='padding: 20px; border: 2px solid #27AE60; border-radius: 10px; background-color: #EAFaf1; text-align: center;'> <h3 style='color: #27AE60; margin: 0;'> The winning team is: {winner}</h3> </div>""", unsafe_allow_html=True)
        st.balloons()
    else:
        st.markdown("<ul style='font-size: 35px; color: #ffcc00;'>No team has reached level 8 yet.</ul>", unsafe_allow_html=True)
    st.markdown("---")
    st.subheader("Titles awarded to specific officials")
    look_actions = datatset[datatset["action"] == "look"]
    st.markdown(f"**The Glutton**(Max ressources) : {datatset.loc[datatset["food"].idxmax(), "id"]}, Value : {(datatset[datatset["action"] == "take"]["id"].value_counts()).max()}")
    st.markdown(f"**The Survivalist**(Max survive) : {datatset.loc[datatset["time"].idxmax(), "id"]}")
    st.markdown(f"**The Mad Mage**(Max fork/incantation) : {datatset["id"].value_counts().idxmax()}, Value : {(datatset[datatset["action"] == "fork"]["id"].value_counts()).max()}")
    st.markdown(f"**The Stalker**(Max i see you) : {look_actions["id"].value_counts().idxmax()}, Value : {(datatset[datatset["action"] == "look"]["id"].value_counts()).max()}")
    st.markdown("---")
    st.caption("Navigation: Use the sidebar on the left to explore the detailed data.")
elif (pages == "Data Analysis"):
    st.title("Welcome to the data analysis section for each agent.")
    sorted_players = sorted(datatset["id"].unique())
    agent_id_ia = st.selectbox('You can select the agent you wish :', sorted_players)
    datatset_agent_choose = datatset[datatset["id"] == agent_id_ia].copy()
    st.markdown("Here is some general information about this agent during this match :")
    cols1, cols2, cols3, cols4 = st.columns(4)
    with cols1:
        st.markdown("<div style='text-align: center;'>Team<br><span style='font-size: 2em; font-weight: bold; color: #2E86C1;'>{}</span></div>".format(datatset_agent_choose["team"].iloc[-1]), unsafe_allow_html=True)
    with cols2:
        st.markdown("<div style='text-align: center;'>Maximum level<br><span style='font-size: 2em; font-weight: bold; color: #ffcc00;'>{}</span></div>".format(datatset_agent_choose["level"].max()), unsafe_allow_html=True)
    with cols3:
        st.markdown("<div style='text-align: center;'>Maximum Food<br><span style='font-size: 2em; font-weight: bold; color: #2E86C1;'>{}</span></div>".format(datatset_agent_choose["food"].max()), unsafe_allow_html=True)
    with cols4:
        st.markdown("<div style='text-align: center;'>Number Actions<br><span style='font-size: 2em; font-weight: bold; color: #ffcc00;'>{}</span></div>".format(len(datatset_agent_choose)), unsafe_allow_html=True)
    st.markdown("---")
    st.title("Several curves to provide a good visual representation for one agent.")
    st.subheader("The evolution of food") 
    food_time = (datatset_agent_choose.groupby("time")["food"].max().reset_index())
    st.line_chart(food_time, x="time", y="food", color=["#338A9D"])
    st.subheader("Player level progression") 
    level_time = (datatset_agent_choose.groupby("time")["level"].max().reset_index())
    st.line_chart(level_time, x="time", y="level", color=["#BE2E1F"])
    st.subheader("Breakdown of actions performed by the agent")
    action_time = (datatset_agent_choose["action"].value_counts().reset_index())
    tab = action_time.columns
    st.bar_chart(action_time, x = tab[0], y = tab[1], color="action")
    st.markdown("---")
    st.title("Comparisons between several agents.")
    st.markdown("---")
    st.subheader("The actions most frequently performed by agents, on average")
    action_count = datatset["action"].value_counts()
    figure = px.pie(values=action_count.values, names=action_count.index, title="Actions repartition", hole=0.3)
    st.plotly_chart(figure, use_container_width=True)
    st.subheader("Evolution of food")
    food_evolution_id = datatset.groupby(["time", "id"])["food"].max().reset_index()
    pivot_graph = food_evolution_id.pivot(index="time", columns="id", values="food")
    st.line_chart(pivot_graph)
    st.subheader("The behavior agents most liked:")
    behavior_count = datatset["mode"].value_counts().reset_index()
    tab = behavior_count.columns
    st.bar_chart(behavior_count, x = tab[0], y = tab[1], color="#ffaa00")
    st.markdown("---")
    st.caption("Gandalf whispers in your ear to use the navigation bar.")
else:
    st.markdown("""<h1 style='color: #2E86C1;'>Authors</h1>""", unsafe_allow_html=True)
    cols1, cols2, cols3, cols4, cols5 = st.columns(5)
    with cols1:       
        st.markdown("<div style='text-align: center;'>", unsafe_allow_html=True)
        st.image("image/louis.webp", width=200, caption="Louis Hector")
        st.markdown("</div>", unsafe_allow_html=True)
    with cols2:
        st.markdown("<div style='text-align: center;'>", unsafe_allow_html=True)
        st.image("image/paul.webp", width=200, caption="Paul Ammeloot")
        st.markdown("</div>", unsafe_allow_html=True)
    with cols3:
        st.markdown("<div style='text-align: center;'>", unsafe_allow_html=True)
        st.image("image/logan.webp", width=200, caption="Logan Capoen")
        st.markdown("</div>", unsafe_allow_html=True)
    with cols4:
        st.markdown("<div style='text-align: center;'>", unsafe_allow_html=True)
        st.image("image/keryan.webp", width=200, caption="Keryan Pollet-druelle")
        st.markdown("</div>", unsafe_allow_html=True)
    with cols5:
        st.markdown("<div style='text-align: center;'>", unsafe_allow_html=True)
        st.image("image/noam.webp", width=200, caption="Noam Bouillet")
        st.markdown("</div>", unsafe_allow_html=True)
    st.markdown("""
    **Zappy Project Objectives**:
    - Server setup
    - Creation of a graphical interface
    - Creation and optimization of algorithms
    ### Links
    - GitHub : (https://github.com/EpitechPGE2-2025/G-YEP-400-LIL-4-1-zappy-6)   
    - Epitech :(https://intra.epitech.eu/module/2025/G-YEP-400/LIL-4-1/acti-701140/)
    """)
    st.markdown("### Technology used")
    st.markdown("""
    - **Python** 
    - **Pandas**
    - **Streamlit**
    - **Plotly**
    """)
    st.markdown("Thank you for you attention")
    st.markdown("---")
    st.caption("You will not get past this page.")

    