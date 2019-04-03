return {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      1,
      2,
      12,
      1,
    },
    {
      1,
      3,
      4,
      1,
    },
    {
      1,
      3,
      5,
      1,
    },
    {
      1,
      3,
      9,
      2,
    },
    {
      1,
      3,
      11,
      2,
    },
    {
      1,
      3,
      37,
      2,
    },
    {
      1,
      3,
      43,
      2,
    },
    {
      1,
      3,
      29,
      2,
    },
    {
      1,
      3,
      21,
      2,
    },
    {
      1,
      3,
      45,
      2,
    },
    {
      1,
      3,
      39,
      2,
    },
    {
      1,
      3,
      41,
      1,
    },
    {
      1,
      3,
      19,
      2,
    },
    {
      1,
      3,
      16,
      2,
    },
    {
      1,
      3,
      12,
      2,
    },
    {
      1,
      3,
      25,
      2,
    },
    {
      1,
      3,
      22,
      2,
    },
    {
      1,
      3,
      48,
      2,
    },
    {
      3,
      1,
      11,
      1,
    },
    {
      3,
      6,
      17,
      1,
    },
    {
      4,
      1,
      8,
      1,
    },
    {
      5,
      1,
      6,
      1,
    },
    {
      6,
      1,
      7,
      1,
    },
    {
      6,
      2,
      7,
      2,
    },
    {
      7,
      1,
      8,
      2,
    },
    {
      8,
      1,
      9,
      3,
    },
    {
      9,
      1,
      28,
      1,
    },
    {
      9,
      1,
      14,
      1,
    },
    {
      10,
      1,
      9,
      4,
    },
    {
      11,
      1,
      9,
      1,
    },
    {
      12,
      1,
      3,
      1,
    },
    {
      13,
      1,
      12,
      3,
    },
    {
      14,
      1,
      43,
      1,
    },
    {
      15,
      1,
      53,
      2,
    },
    {
      16,
      1,
      22,
      1,
    },
    {
      17,
      1,
      16,
      1,
    },
    {
      18,
      1,
      37,
      3,
    },
    {
      19,
      1,
      29,
      1,
    },
    {
      20,
      1,
      37,
      4,
    },
    {
      21,
      1,
      53,
      1,
    },
    {
      22,
      1,
      25,
      1,
    },
    {
      23,
      1,
      16,
      3,
    },
    {
      24,
      1,
      22,
      4,
    },
    {
      25,
      1,
      37,
      1,
    },
    {
      26,
      1,
      22,
      3,
    },
    {
      27,
      1,
      34,
      2,
    },
    {
      28,
      1,
      39,
      1,
    },
    {
      29,
      1,
      45,
      1,
    },
    {
      30,
      1,
      29,
      4,
    },
    {
      31,
      1,
      29,
      3,
    },
    {
      32,
      1,
      40,
      2,
    },
    {
      32,
      2,
      40,
      1,
    },
    {
      33,
      1,
      34,
      1,
    },
    {
      34,
      1,
      28,
      2,
    },
    {
      35,
      1,
      49,
      2,
    },
    {
      36,
      1,
      25,
      3,
    },
    {
      37,
      1,
      19,
      1,
    },
    {
      38,
      1,
      25,
      4,
    },
    {
      40,
      1,
      39,
      3,
    },
    {
      41,
      1,
      32,
      1,
    },
    {
      41,
      1,
      33,
      1,
    },
    {
      42,
      1,
      19,
      3,
    },
    {
      44,
      1,
      19,
      4,
    },
    {
      46,
      1,
      45,
      4,
    },
    {
      47,
      1,
      45,
      3,
    },
    {
      48,
      1,
      49,
      1,
    },
    {
      49,
      1,
      14,
      2,
    },
    {
      50,
      1,
      48,
      1,
    },
    {
      51,
      1,
      43,
      3,
    },
    {
      52,
      1,
      21,
      1,
    },
    {
      53,
      1,
      17,
      2,
    },
  },
  nodeType = 'script',
  nodes = {
    {
      name = 'components/playercontrollercomponent',
    },
    {
      loadArguments = {
        {
          5,
          'Component',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      loadArguments = {
        7,
      },
      name = 'script/impulseall',
    },
    {
      name = 'controller/leftstickdirection',
    },
    {
      name = 'entity/getposition',
    },
    {
      name = 'math/vector3/vector3split',
    },
    {
      name = 'vector2',
    },
    {
      name = 'math/vector2/vector2add',
    },
    {
      name = 'entity/moveto',
    },
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      name = 'entity/clearpath',
    },
    {
      name = 'entity/setstrafing',
    },
    {
      loadArguments = {
        true,
      },
      name = 'boolean',
    },
    {
      name = 'script/condition',
    },
    {
      loadArguments = {
        0.5,
      },
      name = 'number',
    },
    {
      name = 'flow/cooldown',
    },
    {
      name = 'script/condition',
    },
    {
      loadArguments = {
        'missile4',
      },
      name = 'string',
    },
    {
      loadArguments = {
        'mods/gdf/compounds/script/Fire Missile',
      },
      name = 'compound',
    },
    {
      loadArguments = {
        -0.15000000596046,
      },
      name = 'number',
    },
    {
      name = 'controller/axisvalue',
    },
    {
      loadArguments = {
        'mods/gdf/compounds/script/Fire Missile',
      },
      name = 'compound',
    },
    {
      loadArguments = {
        0.050000000745058,
      },
      name = 'number',
    },
    {
      loadArguments = {
        -0.050000000745058,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'mods/gdf/compounds/script/Fire Missile',
      },
      name = 'compound',
    },
    {
      loadArguments = {
        'missile4',
      },
      name = 'string',
    },
    {
      loadArguments = {
        0.5,
      },
      name = 'number',
    },
    {
      name = 'script/condition',
    },
    {
      loadArguments = {
        'mods/gdf/compounds/script/Fire Missile',
      },
      name = 'compound',
    },
    {
      loadArguments = {
        -0.25,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'missile4',
      },
      name = 'string',
    },
    {
      name = 'math/vector2/vector2split',
    },
    {
      name = 'math/vector2/vector2length',
    },
    {
      name = 'math/greaterthan',
    },
    {
      loadArguments = {
        0.5,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'missile4',
      },
      name = 'string',
    },
    {
      loadArguments = {
        'mods/gdf/compounds/script/Fire Missile',
      },
      name = 'compound',
    },
    {
      loadArguments = {
        0.050000000745058,
      },
      name = 'number',
    },
    {
      name = 'entity/setheading',
    },
    {
      loadArguments = {
        'atan(y,x)',
      },
      name = 'math/formula',
    },
    {
      name = 'controller/rightstickdirection',
    },
    {
      loadArguments = {
        'missile4',
      },
      name = 'string',
    },
    {
      name = 'entity/jump',
    },
    {
      loadArguments = {
        0.15000000596046,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'mods/gdf/compounds/script/Fire Missile',
      },
      name = 'compound',
    },
    {
      loadArguments = {
        0.25,
      },
      name = 'number',
    },
    {
      loadArguments = {
        'missile4',
      },
      name = 'string',
    },
    {
      name = 'controller/axisvalue',
    },
    {
      name = 'math/greaterthan',
    },
    {
      loadArguments = {
        'TRIGGERLEFT',
      },
      name = 'string',
    },
    {
      loadArguments = {
        false,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        'TRIGGERRIGHT',
      },
      name = 'string',
    },
    {
      name = 'math/greaterthan',
    },
  },
}